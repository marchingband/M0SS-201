#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Midi messages type
enum midiMsgTypeMaskValue {
    noneMsgTypeMsk          = 0B0000,
    channelVoiceMsgTypeMsk  = 0B0001,
    systemCommonMsgTypeMsk  = 0B0010,
    realTimeMsgTypeMsk      = 0B0100,
    sysExMsgTypeMsk         = 0B1000,
    allMsgTypeMsk           = 0B1111
};

enum midiStatusValue {
    // CHANNEL VOICE
    noteOffStatus         = 0X80,
    noteOnStatus          = 0X90,
    polyKeyPressureStatus = 0XA0,
    controlChangeStatus   = 0XB0,
    programChangeStatus   = 0XC0,
    channelPressureStatus = 0XD0,
    pitchBendStatus       = 0XE0,
    // SYSTEM COMMON
    soxStatus             = 0XF0,
    midiTimeCodeStatus    = 0XF1,
    songPosPointerStatus  = 0XF2,
    songSelectStatus      = 0XF3,
    reserved1Status       = 0XF4,
    reserved2Status       = 0XF5,
    tuneRequestStatus     = 0XF6,
    eoxStatus             = 0XF7,
    // REAL TIME
    timingClockStatus     = 0XF8,
    reserved3Status       = 0XF9,
    startStatus           = 0XFA,
    continueStatus        = 0XFB,
    stopStatus            = 0XFC,
    reserved4Status       = 0XFD,
    activeSensingStatus   = 0XFE,
    systemResetStatus     = 0XFF
};

uint8_t  m_midiMsg[3];
uint8_t  m_midiMsgRealTime; // Used for real time only
uint8_t  m_indexMsgLen = 0;
uint8_t  m_expectedMsgLen = 0;
bool     m_sysExMode = false;
bool     m_sysExError = false;
unsigned m_sysExMsgLen = 0;
unsigned m_sysExindexMsgLen = 0;
bool     m_isByteCaptured=false;
uint8_t     m_readByte = 0;
bool     m_runningStatusPossible=false;
uint8_t  m_midiMsgTypeFilterMsk = noneMsgTypeMsk;
uint8_t  m_midiParsedMsgType    = noneMsgTypeMsk;
uint8_t  m_midiCurrentMsgType   = noneMsgTypeMsk;


// Methods
bool        isSysExMode() ;
bool        wasSysExMode() ;
bool        isSysExError();
bool        isByteCaptured() ;
bool        isMidiStatus(enum midiStatusValue midiStatusValue );
uint8_t     getMidiMsgType() ;
uint8_t     getMidiCurrentMsgType() ;
uint8_t     getMidiMsgLen();
uint8_t *   getMidiMsg();
uint8_t     getByte() ;
unsigned    getSysExMsgLen() ;
void        setMidiMsgFilter(uint8_t );
bool        parse(uint8_t byte );
static uint8_t     getMidiStatusMsgTypeMsk(uint8_t ) ;
static uint8_t     getMidiStatusMsgLen(uint8_t );

const  uint8_t m_systemCommonMsglen[]={
        // SYSTEM COMMON
        0, // soxStatus             = 0XF0,
        2, // midiTimeCodeStatus    = 0XF1,
        3, // songPosPointerStatus  = 0XF2,
        2, // songSelectStatus      = 0XF3,
        0, // reserved1Status       = 0XF4,
        0, // reserved2Status       = 0XF5,
        1, // tuneRequestStatus     = 0XF6,
        0  // eoxStatus             = 0XF7,
};

const  uint8_t m_channelVoiceMsgMsglen[]={
        3, // noteOffStatus         = 0X80,
        3, // noteOnStatus          = 0X90,
        3, // polyKeyPressureStatus = 0XA0,
        3, // controlChangeStatus   = 0XB0,
        2, // programChangeStatus   = 0XC0,
        2, // channelPressureStatus = 0XD0,
        3, // pitchBendStatus       = 0XE0,
};

// Give the current sysex state,
bool isSysExMode() { return m_sysExMode ;}

bool wasSysExMode() {
  return  ( ( m_readByte == eoxStatus && m_isByteCaptured ) || m_sysExError );
}

// Give the status of the last SYSEX transmission.
bool isSysExError() { return m_sysExError ;}

// Used to check if the last byte parsed was captured
bool isByteCaptured() { return m_isByteCaptured; }

// Check the midi status of  a non sysex parsed midi msg
bool isMidiStatus(enum midiStatusValue midiStatus) {

  if ( m_midiParsedMsgType == noneMsgTypeMsk ) return false;  // Only if a parsed msg exists

  // Channel voice msg
  if ( m_midiParsedMsgType == channelVoiceMsgTypeMsk ) return ( ( m_midiMsg[0] & 0xF0 ) == (uint8_t)midiStatus ) ;
  // System Common msg
  if ( m_midiParsedMsgType == systemCommonMsgTypeMsk ) return ( m_midiMsg[0] == (uint8_t)midiStatus ) ;
  // realtime msg
  if ( m_midiParsedMsgType == realTimeMsgTypeMsk ) return ( m_midiMsgRealTime == (uint8_t)midiStatus) ;

  return false;
}

// Return the type of the last parsed midi message
uint8_t  getMidiMsgType() { return m_midiParsedMsgType; }

// Return the type of the currently parsed midi message
uint8_t  getMidiCurrentMsgType() { return m_midiCurrentMsgType; }

// Return the type of a midi status (cf enum)
uint8_t  getMidiStatusMsgTypeMsk(uint8_t midiStatus) {

  if (midiStatus >= 0xF8 ) return realTimeMsgTypeMsk;
  if (midiStatus == 0XF7 || midiStatus == 0xF0 ) return sysExMsgTypeMsk;
  if (midiStatus >= 0xF0 ) return systemCommonMsgTypeMsk;
  if (midiStatus >= 0x80 ) return channelVoiceMsgTypeMsk;

  return noneMsgTypeMsk ;
}

// Return the len of the last parsed midi message, including sysex
uint8_t  getMidiMsgLen() {
  if (m_midiParsedMsgType == sysExMsgTypeMsk )         return getSysExMsgLen() ;
  if (m_midiParsedMsgType == realTimeMsgTypeMsk )      return 1 ;
  if (m_midiParsedMsgType == channelVoiceMsgTypeMsk )  return m_channelVoiceMsgMsglen[ (getMidiMsg()[0] >> 4) - 8 ] ;
  if (m_midiParsedMsgType == systemCommonMsgTypeMsk )  return m_systemCommonMsglen[getMidiMsg()[0] & 0x0F] ;

  return 0;
}

// Return the len of a midistatus message (cf enum)
// Nb: SOX or EOX return always 0.

uint8_t getMidiStatusMsgLen(uint8_t midiStatus) {
  if (midiStatus >= 0xF8 ) return 1;
  if (midiStatus >= 0xF0 ) return m_systemCommonMsglen[midiStatus & 0x0F];
  if (midiStatus >= 0x80 ) return m_channelVoiceMsgMsglen[ (midiStatus >> 4) - 8 ];
  return 0 ;
}

// Return the len of the last Sysex msg.
// This persists until the next sysex.
unsigned getSysExMsgLen() { return m_sysExMsgLen ;}

// Return the parsed message buffered
uint8_t * getMidiMsg() {

    switch (m_midiParsedMsgType) {
      case realTimeMsgTypeMsk:
        return &m_midiMsgRealTime;
        break;
      case sysExMsgTypeMsk:
        return (uint8_t*) NULL;
        break;
    }

   return m_midiMsg ;
}

// Get the last byte parsed
uint8_t getByte() { return m_readByte ;}

// Set filter mask all/none for all midi Msg including Sysex
// For Sysex, the "on the fly" mode is activated by default
// To change that, you must call explicitly setSysExFilter again.
void setMidiMsgFilter(uint8_t midiMsgTypeFilterMsk) {
  m_midiMsgTypeFilterMsk = midiMsgTypeFilterMsk;
}

//////////////////////////////////////////////////////////////////////////////
// midiXParser MIDI PARSER
//----------------------------------------------------------------------------
// The main method.
// It parses midi byte per byte and return true if a message is matching filters.
// Set also the byte capture flag if a byte belong to a filtered message.
//////////////////////////////////////////////////////////////////////////////
bool parse(uint8_t readByte) {

    // Store the passed byte so it can be sent back to serial
    // if not captured
    m_readByte = readByte;
    m_isByteCaptured = false;
    m_sysExError = false;    // Clear any previous sysex error

    // MIDI Message status are starting at 0x80
    if ( readByte >= 0x80  )
    {

      // Check filters.
      m_isByteCaptured = (m_midiMsgTypeFilterMsk  & getMidiStatusMsgTypeMsk(readByte) );

      // SysEx can be terminated abnormally with a midi status.
      // We must check that before applying filters.
       if ( m_sysExMode && m_readByte < 0xF7 ) {
          m_sysExError = true;
          m_sysExMode = false;
          m_sysExMsgLen = m_sysExindexMsgLen;
       }

       // Real time messages must be processed as transparent for all other status
       if  ( readByte >= 0xF8 ) {
            m_midiParsedMsgType = realTimeMsgTypeMsk;
            // NB : m_midiCurrentMsgType can't be used as real time can be mixed with
            // channel voice msg.
            m_midiMsgRealTime = readByte;
            return m_isByteCaptured;
       }

       // Running status not possible at this point
       m_runningStatusPossible=false;

       // Reset current msg type and msg len
       m_midiCurrentMsgType = noneMsgTypeMsk;
       m_indexMsgLen = m_expectedMsgLen = 0;

       // Apply filter
       if (!m_isByteCaptured) return false;

       // Start SYSEX ---------------------------------------------------------

       // END OF Sysex.
       if ( readByte == eoxStatus ) {
            m_sysExMsgLen = m_sysExindexMsgLen;
            if (m_sysExMode ) {
               m_midiParsedMsgType = sysExMsgTypeMsk;
               m_sysExMode = false;
               return true;
            } // Isolated EOX without SOX.
            m_sysExMsgLen = 0;
            m_sysExError = true;
            m_midiCurrentMsgType = sysExMsgTypeMsk;
            return false;
       }

       // Start SYSEX
       if ( readByte == soxStatus ) {
              m_sysExMode = true;
              m_sysExindexMsgLen = 0;
              m_midiCurrentMsgType = sysExMsgTypeMsk;
              return false;
       }
       // Start midi msg ------------------------------------------------------

       // get the len of the midi msg to parse minus status
       m_midiMsg[0] = readByte;
       m_expectedMsgLen = getMidiStatusMsgLen(readByte) ;
       m_indexMsgLen = m_expectedMsgLen - 1;

      // Channel messages between 0x80 and 0xEF -------------------------------
      if ( readByte <= 0xEF ) {
          m_midiCurrentMsgType = channelVoiceMsgTypeMsk;
      }

      // System common messages between 0xF0 and 0xF7 -------------------------
      // but SOX / EOX
      else {
          m_midiCurrentMsgType = systemCommonMsgTypeMsk;
          // Case of 1 byte len midi msg (Tune request)
          if ( m_indexMsgLen == 0 ) {
            m_midiParsedMsgType = m_midiCurrentMsgType;
            return true;
          }
      }
    }

    // Midi Data from 00 to 0X7F ----------------------------------------------
    else {

          // Capture the SYSEX message if filter is set
          // If m_sysExBufferSize is 0, do not store
          if (m_sysExMode ) {
              m_sysExindexMsgLen++;
              m_isByteCaptured = true;
              return false;
          }

          // "Pure" midi message data
          // check if Running status set and if so, generate a true midi channel msg with
          // the previous one. Possible only if filters matchs.
          if (m_runningStatusPossible) {
                m_indexMsgLen = m_expectedMsgLen-1;
                m_runningStatusPossible = false;
          }

          // Len was set only if filters matched before
          if ( m_indexMsgLen ) {

            m_midiMsg[m_expectedMsgLen-m_indexMsgLen] = readByte;
            m_isByteCaptured = true;
            m_indexMsgLen -- ;
            // Message complete ?
            // Enable running status if it is a message channel.
            if (m_indexMsgLen == 0) {
                m_midiParsedMsgType = m_midiCurrentMsgType;
                if (m_midiParsedMsgType == channelVoiceMsgTypeMsk) {
                  m_runningStatusPossible = true;
                }
                return true;
            }
          }

     } // Midi data from 00 to 0X7F

     // All other data here are purely ignored.
     // In respect of the MIDI specifications.

    return false;
}

void init_midi_parser(void){
  setMidiMsgFilter( channelVoiceMsgTypeMsk );
}