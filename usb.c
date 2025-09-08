/*
 * Copyright (c) 2022, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "usbh_core.h"
#include "usb_audio.h"
#include <stdio.h>
#include <FreeRTOS.h>

// declare from midi.c
void usb_midi_in(char ch);

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t midi_buffer[512];
int midi_run = 0;

struct usbh_midi
{
    struct usbh_hubport *hport;

    uint8_t intf;        /* interface number */
    struct usb_endpoint_descriptor *bulkin;  /* BULK IN endpoint */
    struct usb_endpoint_descriptor *bulkout; /* BULK OUT endpoint */
    struct usbh_urb bulkin_urb;
    struct usbh_urb bulkout_urb;
    uint8_t minor;
};

#define DEV_FORMAT "/dev/midi"

void usbh_midi_callback(void *arg, int nbytes)
{
    struct usbh_midi *midi_class = (struct usbh_midi *)arg;

    if (nbytes > 0)
    {
        for (size_t i = 0; i < nbytes; i++)
        {
            USB_LOG_RAW("0x%02x ", midi_buffer[i]);
        }
        USB_LOG_RAW("nbytes:%d\r\n", nbytes);
        if(midi_run == 1)
        {
            for (int i=0; i< nbytes; i+=4)
            {
                switch(midi_buffer[i]){
                    case 0x8: // note off
                    case 0x9: // note on
                    case 0xB: // cc
                    case 0xE: // pitch bend
                        // all these are 3 bytes
                        usb_midi_in(midi_buffer[i + 1]);
                        usb_midi_in(midi_buffer[i + 2]);
                        usb_midi_in(midi_buffer[i + 3]);
                        break;
                    default:
                        break;
                }
            }
        }
        usbh_submit_urb(&midi_class->bulkin_urb);
    }
}


static void usbh_midi_thread(void *argument){
    int ret;
    struct usbh_midi *midi_class;
    // printf("strt usbh_midi_thread");

    // clang-format off
find_class:
    // clang-format on
    while ((midi_class = (struct usbh_midi *)usbh_find_class_instance("/dev/midi")) == NULL) {
        goto delete;
    }

    memset(midi_buffer, 0, 512);

    usbh_bulk_urb_fill(&midi_class->bulkin_urb, midi_class->hport, midi_class->bulkin, midi_buffer, 64, 3000, usbh_midi_callback, midi_class);
    ret = usbh_submit_urb(&midi_class->bulkin_urb);
    if ((ret < 0) && (ret != -16))
    {
        USB_LOG_RAW("bulk in error,ret:%d\r\n", ret);
        // goto find_class;
    } else {

    }

    // clang-format off
delete: 
    usb_osal_thread_delete(NULL);
    // clang-format on
}


void usbh_midi_run(struct usbh_midi *midi_class)
{
    usb_osal_thread_create("usbh_midi", 2048, CONFIG_USBHOST_PSC_PRIO + 1, usbh_midi_thread, NULL);
}

static int usbh_midi_connect(struct usbh_hubport *hport, uint8_t intf)
{
    // printf("class connect");
    // struct usbh_endpoint_cfg ep_cfg = {0};
    struct usb_endpoint_descriptor *ep_desc;
    int ret;

    struct usbh_midi *midi_class = usb_malloc(sizeof(struct usbh_midi));
    if (midi_class == NULL)
    {
        USB_LOG_ERR("Fail to alloc midi_class\r\n");
        return -1;
    }

    memset(midi_class, 0, sizeof(struct usbh_midi));
    midi_class->hport = hport;
    midi_class->intf = intf;

    hport->config.intf[intf].priv = midi_class;

    for (uint8_t i = 0; i < hport->config.intf[intf].altsetting[0].intf_desc.bNumEndpoints; i++)
    {
        ep_desc = &hport->config.intf[intf].altsetting[0].ep[i].ep_desc;

        if (ep_desc->bEndpointAddress & 0x80)
        {
            // usbh_hport_activate_epx(&midi_class->bulkin, hport, ep_desc);
            USBH_EP_INIT(midi_class->bulkin, ep_desc);
        }
        else
        {
        //     // usbh_hport_activate_epx(&midi_class->bulkout, hport, ep_desc);
            USBH_EP_INIT(midi_class->bulkout, ep_desc);
        }
    }

    strncpy(hport->config.intf[intf].devname, DEV_FORMAT, CONFIG_USBHOST_DEV_NAMELEN);
    USB_LOG_INFO("Register MIDI Class:%s\r\n", hport->config.intf[intf].devname);

    midi_run = 1;

    usbh_midi_run(midi_class);
    // return ret;
    return 0;
}

static int usbh_midi_disconnect(struct usbh_hubport *hport, uint8_t intf)
{
    // printf("class disconnect");
    int ret = 0;

    struct usbh_midi *midi_class = (struct usbh_midi *)hport->config.intf[intf].priv;

    if (midi_class)
    {
        if (midi_class->bulkin)
        {
            // printf("usbh_kill_urb in ");
            usbh_kill_urb(&midi_class->bulkin_urb);
        } else {
            // printf("bulkin_urb not found");
        }

        if (midi_class->bulkout)
        {
            // printf("usbh_kill_urb out");
            usbh_kill_urb(&midi_class->bulkout_urb);
        } else {
            // printf("bulkout_urb not found");
        }

        if (hport->config.intf[intf].devname[0] != '\0')
        {
            // USB_LOG_INFO("Unregister MIDI Class:%s\r\n", hport->config.intf[intf].devname);
            // printf("Unregister MIDI Class:%s\r\n", hport->config.intf[intf].devname);
            // usbh_cdc_acm_stop(midi_class);
            midi_run = 0;
        } else {
            // printf("devname not found");
        }

        memset(midi_class, 0, sizeof(struct usbh_midi));
        usb_free(midi_class);
    } else {
        // printf("midi_class not found");
    }

    return ret;
}

static const struct usbh_class_driver midi_streaming_class_driver = {
    .driver_name = "midi_streaming",
    .connect = usbh_midi_connect,
    .disconnect = usbh_midi_disconnect};

CLASS_INFO_DEFINE const struct usbh_class_info midi_streaming_class_info = {
    .match_flags = USB_CLASS_MATCH_INTF_CLASS | USB_CLASS_MATCH_INTF_SUBCLASS,
    .class = USB_DEVICE_CLASS_AUDIO,
    .subclass = AUDIO_SUBCLASS_MIDISTREAMING,
    .protocol = 0x00,
    .vid = 0x00,
    .pid = 0x00,
    .class_driver = &midi_streaming_class_driver};

/* -------------------------- */

static void _usbh_midi_thread(void *argument)
{
    int ret;
    struct usbh_midi *midi_class;
    // printf("strt usbh_midi_thread");

    while (1)
    {
    find_class:
        midi_class = (struct usbh_midi *)usbh_find_class_instance("/dev/midi");
        if (midi_class == NULL)
        {
            USB_LOG_RAW("do not find /dev/midi\r\n");
            usb_osal_msleep(1000);
            continue;
        }
        memset(midi_buffer, 0, 512);

        // usbh_bulk_urb_fill(&bulkin_urb, midi_class->bulkin, midi_buffer, 64, 3000, NULL, NULL);
        // ret = usbh_submit_urb(&bulkin_urb);
        // if (ret < 0)
        // {
        //     USB_LOG_RAW("bulk in error,ret:%d\r\n", ret);
        // }
        // else
        // {
        //     USB_LOG_RAW("recv over:%d\r\n", bulkin_urb.actual_length);
        //     for (size_t i = 0; i < bulkin_urb.actual_length; i++)
        //     {
        //         USB_LOG_RAW("0x%02x ", midi_buffer[i]);
        //     }
        // }

        // USB_LOG_RAW("\r\n");
        // const uint8_t data1[10] = {0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x08, 0x14};

        // memcpy(midi_buffer, data1, 8);
        // usbh_bulk_urb_fill(&bulkout_urb, midi_class->bulkout, midi_buffer, 8, 3000, NULL, NULL);
        // ret = usbh_submit_urb(&bulkout_urb);
        // if (ret < 0)
        // {
        //     USB_LOG_RAW("bulk out error,ret:%d\r\n", ret);
        // }
        // else
        // {
        //     USB_LOG_RAW("send over:%d\r\n", bulkout_urb.actual_length);
        // }

        // usbh_bulk_urb_fill(&bulkin_urb, midi_class->bulkin, midi_buffer, 64, 3000, usbh_midi_callback, midi_class);
        // ret = usbh_submit_urb(&bulkin_urb);
        // if (ret < 0)
        // {
        //     USB_LOG_RAW("bulk in error,ret:%d\r\n", ret);
        // }
        // else
        // {
        // }

        while (1)
        {
            midi_class = (struct usbh_midi *)usbh_find_class_instance(DEV_FORMAT);
            if (midi_class == NULL)
            {
                goto find_class;
            }
            else
            {
                usbh_bulk_urb_fill(&midi_class->bulkin_urb, midi_class->hport, midi_class->bulkin, midi_buffer, 64, 100, usbh_midi_callback, midi_class);
                // usbh_bulk_urb_fill(&bulkin_urb, midi_class->bulkin, midi_buffer, 64, 0, usbh_midi_callback, midi_class);
                ret = usbh_submit_urb(&midi_class->bulkin_urb);
                if ((ret < 0) && (ret != -16))
                {
                    USB_LOG_RAW("bulk in error,ret:%d\r\n", ret);
                }
            }
            usb_osal_msleep(10);
        }
    }
}

void usb_midi_init(void)
{
    usb_osal_thread_create("usbh_midi", 2048, CONFIG_USBHOST_PSC_PRIO + 1, usbh_midi_thread, NULL);
}