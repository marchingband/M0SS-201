/* ------------------------------------------------------------
name: "M0SS-201-krell"
Code generated with Faust 2.68.6 (https://faust.grame.fr)
Compilation options: -lang c -ct 1 -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 


#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
} mydspSIG0;

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)calloc(1, sizeof(mydspSIG0)); }
static void deletemydspSIG0(mydspSIG0* dsp) { free(dsp); }

int getNumInputsmydspSIG0(mydspSIG0* RESTRICT dsp) {
	return 0;
}
int getNumOutputsmydspSIG0(mydspSIG0* RESTRICT dsp) {
	return 1;
}

static void instanceInitmydspSIG0(mydspSIG0* dsp, int sample_rate) {
}

static void fillmydspSIG0(mydspSIG0* dsp, int count, float* table) {
	/* C99 loop */
	{
		int i1;
		for (i1 = 0; i1 < count; i1 = i1 + 1) {
			table[i1] = 0.0f;
		}
	}
}

typedef struct {
	int iVec6[2];
	int iRec33[2];
} mydspSIG1;

static mydspSIG1* newmydspSIG1() { return (mydspSIG1*)calloc(1, sizeof(mydspSIG1)); }
static void deletemydspSIG1(mydspSIG1* dsp) { free(dsp); }

int getNumInputsmydspSIG1(mydspSIG1* RESTRICT dsp) {
	return 0;
}
int getNumOutputsmydspSIG1(mydspSIG1* RESTRICT dsp) {
	return 1;
}

static void instanceInitmydspSIG1(mydspSIG1* dsp, int sample_rate) {
	/* C99 loop */
	{
		int l33;
		for (l33 = 0; l33 < 2; l33 = l33 + 1) {
			dsp->iVec6[l33] = 0;
		}
	}
	/* C99 loop */
	{
		int l34;
		for (l34 = 0; l34 < 2; l34 = l34 + 1) {
			dsp->iRec33[l34] = 0;
		}
	}
}

static void fillmydspSIG1(mydspSIG1* dsp, int count, float* table) {
	/* C99 loop */
	{
		int i2;
		for (i2 = 0; i2 < count; i2 = i2 + 1) {
			dsp->iVec6[0] = 1;
			dsp->iRec33[0] = (dsp->iVec6[1] + dsp->iRec33[1]) % 65536;
			table[i2] = sinf(9.58738e-05f * (float)(dsp->iRec33[0]));
			dsp->iVec6[1] = dsp->iVec6[0];
			dsp->iRec33[1] = dsp->iRec33[0];
		}
	}
}

typedef struct {
	int iRec35[2];
} mydspSIG2;

static mydspSIG2* newmydspSIG2() { return (mydspSIG2*)calloc(1, sizeof(mydspSIG2)); }
static void deletemydspSIG2(mydspSIG2* dsp) { free(dsp); }

int getNumInputsmydspSIG2(mydspSIG2* RESTRICT dsp) {
	return 0;
}
int getNumOutputsmydspSIG2(mydspSIG2* RESTRICT dsp) {
	return 1;
}

static void instanceInitmydspSIG2(mydspSIG2* dsp, int sample_rate) {
	/* C99 loop */
	{
		int l35;
		for (l35 = 0; l35 < 2; l35 = l35 + 1) {
			dsp->iRec35[l35] = 0;
		}
	}
}

static void fillmydspSIG2(mydspSIG2* dsp, int count, float* table) {
	/* C99 loop */
	{
		int i3;
		for (i3 = 0; i3 < count; i3 = i3 + 1) {
			dsp->iRec35[0] = dsp->iRec35[1] + 1;
			table[i3] = logf(19.540567f * (float)(dsp->iRec35[0] + -1) + 1e+01f);
			dsp->iRec35[1] = dsp->iRec35[0];
		}
	}
}

static float mydsp_faustpower2_f(float value) {
	return value * value;
}
static float ftbl1mydspSIG1[65536];
static float ftbl2mydspSIG2[1024];

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

typedef struct {
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fVslider0;
	float fConst2;
	int iVec0[2];
	float fRec0[2];
	FAUSTFLOAT fVslider1;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	float fConst3;
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	float ftbl0[32];
	FAUSTFLOAT fVslider6;
	float fConst4;
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	FAUSTFLOAT fVslider9;
	FAUSTFLOAT fVslider10;
	float fRec11[2];
	float fVec1[2];
	int iRec10[2];
	FAUSTFLOAT fVslider11;
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fVslider13;
	FAUSTFLOAT fVslider14;
	int iRec13[2];
	float fRec12[2];
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fVslider16;
	FAUSTFLOAT fVslider17;
	FAUSTFLOAT fVslider18;
	float fRec15[2];
	float fRec9[2];
	FAUSTFLOAT fVslider19;
	FAUSTFLOAT fVslider20;
	FAUSTFLOAT fVslider21;
	FAUSTFLOAT fVslider22;
	float fRec8[2];
	float fRec3[2];
	float fRec4[2];
	FAUSTFLOAT fVslider23;
	FAUSTFLOAT fButton0;
	float fVec2[2];
	int iRec16[2];
	FAUSTFLOAT fVslider24;
	FAUSTFLOAT fVslider25;
	FAUSTFLOAT fVslider26;
	FAUSTFLOAT fVslider27;
	FAUSTFLOAT fVslider28;
	FAUSTFLOAT fVslider29;
	FAUSTFLOAT fVslider30;
	FAUSTFLOAT fVslider31;
	float fRec17[2];
	int iRec21[2];
	int iRec19[2];
	int iVec3[2];
	float fRec22[2];
	float fRec20[2];
	int iRec23[2];
	float fRec25[2];
	float fRec24[2];
	float fRec5[2];
	float fRec6[2];
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fEntry0;
	FAUSTFLOAT fVslider32;
	float fRec28[2];
	FAUSTFLOAT fVslider33;
	FAUSTFLOAT fVslider34;
	FAUSTFLOAT fVslider35;
	float fRec26[2];
	float fRec29[2];
	float fConst5;
	float fRec30[2];
	float fVec4[2];
	int IOTA0;
	float fVec5[4096];
	float fConst6;
	float fConst7;
	float fRec31[2];
	float fRec7[2];
	FAUSTFLOAT fVbargraph0;
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVslider36;
	FAUSTFLOAT fVbargraph2;
	FAUSTFLOAT fVslider37;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fVslider38;
	FAUSTFLOAT fVslider39;
	FAUSTFLOAT fVslider40;
	FAUSTFLOAT fVslider41;
	FAUSTFLOAT fVslider42;
	FAUSTFLOAT fVslider43;
	FAUSTFLOAT fVslider44;
	FAUSTFLOAT fVslider45;
	FAUSTFLOAT fVslider46;
	float fRec34[2];
	float fRec37[2];
	float fVec7[2];
	float fVec8[4096];
	float fRec36[2];
	FAUSTFLOAT fVslider47;
	float fRec38[2];
	FAUSTFLOAT fVslider48;
	FAUSTFLOAT fVslider49;
	FAUSTFLOAT fVslider50;
	FAUSTFLOAT fVslider51;
	float fRec39[2];
	FAUSTFLOAT fVslider52;
	FAUSTFLOAT fVslider53;
	FAUSTFLOAT fVslider54;
	float fVec9[2];
	float fRec32[2];
	float fRec2[2];
	FAUSTFLOAT fVbargraph3;
	float fVec10[16384];
	FAUSTFLOAT fVslider55;
	float fConst8;
	float fRec40[2];
	float fRec41[2];
	float fRec42[2];
	float fRec43[2];
	float fRec1[2];
} mydsp;

mydsp* newmydsp() { 
	mydsp* dsp = (mydsp*)calloc(1, sizeof(mydsp));
	return dsp;
}

void deletemydsp(mydsp* dsp) { 
	free(dsp);
}

void metadatamydsp(MetaGlue* m) { 
	m->declare(m->metaInterface, "aanl.lib/name", "Faust Antialiased Nonlinearities");
	m->declare(m->metaInterface, "aanl.lib/version", "1.3.0");
	m->declare(m->metaInterface, "basics.lib/counter:author", "Stephane Letz");
	m->declare(m->metaInterface, "basics.lib/name", "Faust Basic Element Library");
	m->declare(m->metaInterface, "basics.lib/tabulate:author", "Stephane Letz");
	m->declare(m->metaInterface, "basics.lib/tabulateNd", "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
	m->declare(m->metaInterface, "basics.lib/version", "1.11.1");
	m->declare(m->metaInterface, "compile_options", "-lang c -ct 1 -es 1 -mcd 16 -single -ftz 0");
	m->declare(m->metaInterface, "delays.lib/name", "Faust Delay Library");
	m->declare(m->metaInterface, "delays.lib/version", "1.1.0");
	m->declare(m->metaInterface, "envelopes.lib/adsr_bias:author", "Andrew John March and David Braun");
	m->declare(m->metaInterface, "envelopes.lib/adsr_bias:licence", "STK-4.3");
	m->declare(m->metaInterface, "envelopes.lib/author", "GRAME");
	m->declare(m->metaInterface, "envelopes.lib/copyright", "GRAME");
	m->declare(m->metaInterface, "envelopes.lib/license", "LGPL with exception");
	m->declare(m->metaInterface, "envelopes.lib/name", "Faust Envelope Library");
	m->declare(m->metaInterface, "envelopes.lib/version", "1.3.0");
	m->declare(m->metaInterface, "filename", "M0SS-201-krell.dsp");
	m->declare(m->metaInterface, "filters.lib/dcblocker:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/lowpass:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/lowpass:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/name", "Faust Filters Library");
	m->declare(m->metaInterface, "filters.lib/pole:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/pole:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/tf1:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/tf1:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/tf1s:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/tf1s:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/version", "1.3.0");
	m->declare(m->metaInterface, "filters.lib/zero:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/zero:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "interpolators.lib/interpolate_cubic:author", "Stéphane Letz");
	m->declare(m->metaInterface, "interpolators.lib/interpolate_cubic:licence", "MIT");
	m->declare(m->metaInterface, "interpolators.lib/name", "Faust Interpolator Library");
	m->declare(m->metaInterface, "interpolators.lib/remap:author", "David Braun");
	m->declare(m->metaInterface, "interpolators.lib/version", "1.3.0");
	m->declare(m->metaInterface, "maths.lib/author", "GRAME");
	m->declare(m->metaInterface, "maths.lib/copyright", "GRAME");
	m->declare(m->metaInterface, "maths.lib/license", "LGPL with exception");
	m->declare(m->metaInterface, "maths.lib/name", "Faust Math Library");
	m->declare(m->metaInterface, "maths.lib/version", "2.7.0");
	m->declare(m->metaInterface, "name", "M0SS-201-krell");
	m->declare(m->metaInterface, "noises.lib/name", "Faust Noise Generator Library");
	m->declare(m->metaInterface, "noises.lib/version", "1.4.0");
	m->declare(m->metaInterface, "options", "[midi:on][nvoices:mono]");
	m->declare(m->metaInterface, "oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
	m->declare(m->metaInterface, "oscillators.lib/lf_sawpos:licence", "STK-4.3");
	m->declare(m->metaInterface, "oscillators.lib/lf_triangle:author", "Bart Brouns");
	m->declare(m->metaInterface, "oscillators.lib/lf_triangle:licence", "STK-4.3");
	m->declare(m->metaInterface, "oscillators.lib/name", "Faust Oscillator Library");
	m->declare(m->metaInterface, "oscillators.lib/saw1:author", "Bart Brouns");
	m->declare(m->metaInterface, "oscillators.lib/saw1:licence", "STK-4.3");
	m->declare(m->metaInterface, "oscillators.lib/saw2ptr:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "oscillators.lib/saw2ptr:license", "STK-4.3");
	m->declare(m->metaInterface, "oscillators.lib/sawN:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "oscillators.lib/sawN:license", "STK-4.3");
	m->declare(m->metaInterface, "oscillators.lib/version", "1.4.0");
	m->declare(m->metaInterface, "platform.lib/name", "Generic Platform Library");
	m->declare(m->metaInterface, "platform.lib/version", "1.3.0");
	m->declare(m->metaInterface, "quantizers.lib/name", "Faust Frequency Quantization Library");
	m->declare(m->metaInterface, "quantizers.lib/version", "1.1.0");
	m->declare(m->metaInterface, "signals.lib/name", "Faust Signal Routing Library");
	m->declare(m->metaInterface, "signals.lib/version", "1.5.0");
}

int getSampleRatemydsp(mydsp* RESTRICT dsp) {
	return dsp->fSampleRate;
}

int getNumInputsmydsp(mydsp* RESTRICT dsp) {
	return 0;
}
int getNumOutputsmydsp(mydsp* RESTRICT dsp) {
	return 2;
}

void classInitmydsp(int sample_rate) {
	mydspSIG1* sig1 = newmydspSIG1();
	instanceInitmydspSIG1(sig1, sample_rate);
	fillmydspSIG1(sig1, 65536, ftbl1mydspSIG1);
	mydspSIG2* sig2 = newmydspSIG2();
	instanceInitmydspSIG2(sig2, sample_rate);
	fillmydspSIG2(sig2, 1024, ftbl2mydspSIG2);
	deletemydspSIG1(sig1);
	deletemydspSIG2(sig2);
}

void instanceResetUserInterfacemydsp(mydsp* dsp) {
	dsp->fVslider0 = (FAUSTFLOAT)(0.4f);
	dsp->fVslider1 = (FAUSTFLOAT)(0.208f);
	dsp->fVslider2 = (FAUSTFLOAT)(0.647f);
	dsp->fVslider3 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider4 = (FAUSTFLOAT)(1.0f);
	dsp->fVslider5 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider6 = (FAUSTFLOAT)(2.0f);
	dsp->fVslider7 = (FAUSTFLOAT)(0.1f);
	dsp->fVslider8 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider9 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider10 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider11 = (FAUSTFLOAT)(8.0f);
	dsp->fVslider12 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider13 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider14 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider15 = (FAUSTFLOAT)(0.85f);
	dsp->fVslider16 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider17 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider18 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider19 = (FAUSTFLOAT)(0.01f);
	dsp->fVslider20 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider21 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider22 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider23 = (FAUSTFLOAT)(2.0f);
	dsp->fButton0 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider24 = (FAUSTFLOAT)(0.01f);
	dsp->fVslider25 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider26 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider27 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider28 = (FAUSTFLOAT)(0.625f);
	dsp->fVslider29 = (FAUSTFLOAT)(0.362f);
	dsp->fVslider30 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider31 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider0 = (FAUSTFLOAT)(0.0f);
	dsp->fCheckbox0 = (FAUSTFLOAT)(0.0f);
	dsp->fCheckbox1 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider1 = (FAUSTFLOAT)(0.074f);
	dsp->fHslider2 = (FAUSTFLOAT)(-0.017f);
	dsp->fEntry0 = (FAUSTFLOAT)(6e+01f);
	dsp->fVslider32 = (FAUSTFLOAT)(0.001f);
	dsp->fVslider33 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider34 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider35 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider36 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider37 = (FAUSTFLOAT)(1.0f);
	dsp->fHslider3 = (FAUSTFLOAT)(1.0f);
	dsp->fCheckbox2 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider4 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider5 = (FAUSTFLOAT)(1.0f);
	dsp->fHslider6 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider38 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider39 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider40 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider41 = (FAUSTFLOAT)(1.0f);
	dsp->fVslider42 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider43 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider44 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider45 = (FAUSTFLOAT)(0.555f);
	dsp->fVslider46 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider47 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider48 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider49 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider50 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider51 = (FAUSTFLOAT)(1.56f);
	dsp->fVslider52 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider53 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider54 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider55 = (FAUSTFLOAT)(6487.0f);
}

void instanceClearmydsp(mydsp* dsp) {
	/* C99 loop */
	{
		int l0;
		for (l0 = 0; l0 < 2; l0 = l0 + 1) {
			dsp->iVec0[l0] = 0;
		}
	}
	/* C99 loop */
	{
		int l1;
		for (l1 = 0; l1 < 2; l1 = l1 + 1) {
			dsp->fRec0[l1] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l2;
		for (l2 = 0; l2 < 2; l2 = l2 + 1) {
			dsp->fRec11[l2] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l3;
		for (l3 = 0; l3 < 2; l3 = l3 + 1) {
			dsp->fVec1[l3] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l4;
		for (l4 = 0; l4 < 2; l4 = l4 + 1) {
			dsp->iRec10[l4] = 0;
		}
	}
	/* C99 loop */
	{
		int l5;
		for (l5 = 0; l5 < 2; l5 = l5 + 1) {
			dsp->iRec13[l5] = 0;
		}
	}
	/* C99 loop */
	{
		int l6;
		for (l6 = 0; l6 < 2; l6 = l6 + 1) {
			dsp->fRec12[l6] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l7;
		for (l7 = 0; l7 < 2; l7 = l7 + 1) {
			dsp->fRec15[l7] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l8;
		for (l8 = 0; l8 < 2; l8 = l8 + 1) {
			dsp->fRec9[l8] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l9;
		for (l9 = 0; l9 < 2; l9 = l9 + 1) {
			dsp->fRec8[l9] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l10;
		for (l10 = 0; l10 < 2; l10 = l10 + 1) {
			dsp->fRec3[l10] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l11;
		for (l11 = 0; l11 < 2; l11 = l11 + 1) {
			dsp->fRec4[l11] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l12;
		for (l12 = 0; l12 < 2; l12 = l12 + 1) {
			dsp->fVec2[l12] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l13;
		for (l13 = 0; l13 < 2; l13 = l13 + 1) {
			dsp->iRec16[l13] = 0;
		}
	}
	/* C99 loop */
	{
		int l14;
		for (l14 = 0; l14 < 2; l14 = l14 + 1) {
			dsp->fRec17[l14] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l15;
		for (l15 = 0; l15 < 2; l15 = l15 + 1) {
			dsp->iRec21[l15] = 0;
		}
	}
	/* C99 loop */
	{
		int l16;
		for (l16 = 0; l16 < 2; l16 = l16 + 1) {
			dsp->iRec19[l16] = 0;
		}
	}
	/* C99 loop */
	{
		int l17;
		for (l17 = 0; l17 < 2; l17 = l17 + 1) {
			dsp->iVec3[l17] = 0;
		}
	}
	/* C99 loop */
	{
		int l18;
		for (l18 = 0; l18 < 2; l18 = l18 + 1) {
			dsp->fRec22[l18] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l19;
		for (l19 = 0; l19 < 2; l19 = l19 + 1) {
			dsp->fRec20[l19] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l20;
		for (l20 = 0; l20 < 2; l20 = l20 + 1) {
			dsp->iRec23[l20] = 0;
		}
	}
	/* C99 loop */
	{
		int l21;
		for (l21 = 0; l21 < 2; l21 = l21 + 1) {
			dsp->fRec25[l21] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l22;
		for (l22 = 0; l22 < 2; l22 = l22 + 1) {
			dsp->fRec24[l22] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l23;
		for (l23 = 0; l23 < 2; l23 = l23 + 1) {
			dsp->fRec5[l23] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l24;
		for (l24 = 0; l24 < 2; l24 = l24 + 1) {
			dsp->fRec6[l24] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l25;
		for (l25 = 0; l25 < 2; l25 = l25 + 1) {
			dsp->fRec28[l25] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l26;
		for (l26 = 0; l26 < 2; l26 = l26 + 1) {
			dsp->fRec26[l26] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l27;
		for (l27 = 0; l27 < 2; l27 = l27 + 1) {
			dsp->fRec29[l27] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l28;
		for (l28 = 0; l28 < 2; l28 = l28 + 1) {
			dsp->fRec30[l28] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l29;
		for (l29 = 0; l29 < 2; l29 = l29 + 1) {
			dsp->fVec4[l29] = 0.0f;
		}
	}
	dsp->IOTA0 = 0;
	/* C99 loop */
	{
		int l30;
		for (l30 = 0; l30 < 4096; l30 = l30 + 1) {
			dsp->fVec5[l30] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l31;
		for (l31 = 0; l31 < 2; l31 = l31 + 1) {
			dsp->fRec31[l31] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l32;
		for (l32 = 0; l32 < 2; l32 = l32 + 1) {
			dsp->fRec7[l32] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l36;
		for (l36 = 0; l36 < 2; l36 = l36 + 1) {
			dsp->fRec34[l36] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l37;
		for (l37 = 0; l37 < 2; l37 = l37 + 1) {
			dsp->fRec37[l37] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l38;
		for (l38 = 0; l38 < 2; l38 = l38 + 1) {
			dsp->fVec7[l38] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l39;
		for (l39 = 0; l39 < 4096; l39 = l39 + 1) {
			dsp->fVec8[l39] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l40;
		for (l40 = 0; l40 < 2; l40 = l40 + 1) {
			dsp->fRec36[l40] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l41;
		for (l41 = 0; l41 < 2; l41 = l41 + 1) {
			dsp->fRec38[l41] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l42;
		for (l42 = 0; l42 < 2; l42 = l42 + 1) {
			dsp->fRec39[l42] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l43;
		for (l43 = 0; l43 < 2; l43 = l43 + 1) {
			dsp->fVec9[l43] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l44;
		for (l44 = 0; l44 < 2; l44 = l44 + 1) {
			dsp->fRec32[l44] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l45;
		for (l45 = 0; l45 < 2; l45 = l45 + 1) {
			dsp->fRec2[l45] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l46;
		for (l46 = 0; l46 < 16384; l46 = l46 + 1) {
			dsp->fVec10[l46] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l47;
		for (l47 = 0; l47 < 2; l47 = l47 + 1) {
			dsp->fRec40[l47] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l48;
		for (l48 = 0; l48 < 2; l48 = l48 + 1) {
			dsp->fRec41[l48] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l49;
		for (l49 = 0; l49 < 2; l49 = l49 + 1) {
			dsp->fRec42[l49] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l50;
		for (l50 = 0; l50 < 2; l50 = l50 + 1) {
			dsp->fRec43[l50] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l51;
		for (l51 = 0; l51 < 2; l51 = l51 + 1) {
			dsp->fRec1[l51] = 0.0f;
		}
	}
}

void instanceConstantsmydsp(mydsp* dsp, int sample_rate) {
	dsp->fSampleRate = sample_rate;
	dsp->fConst0 = fminf(1.92e+05f, fmaxf(1.0f, (float)(dsp->fSampleRate)));
	dsp->fConst1 = 44.1f / dsp->fConst0;
	dsp->fConst2 = 1.0f - dsp->fConst1;
	dsp->fConst3 = 3.1415927f / dsp->fConst0;
	mydspSIG0* sig0 = newmydspSIG0();
	instanceInitmydspSIG0(sig0, sample_rate);
	fillmydspSIG0(sig0, 32, dsp->ftbl0);
	dsp->fConst4 = 1.0f / dsp->fConst0;
	dsp->fConst5 = 0.25f * dsp->fConst0;
	dsp->fConst6 = 0.5f * dsp->fConst0;
	dsp->fConst7 = 4.0f / dsp->fConst0;
	dsp->fConst8 = 1e+01f / dsp->fConst0;
	deletemydspSIG0(sig0);
}

void instanceInitmydsp(mydsp* dsp, int sample_rate) {
	instanceConstantsmydsp(dsp, sample_rate);
	instanceResetUserInterfacemydsp(dsp);
	instanceClearmydsp(dsp);
}

void initmydsp(mydsp* dsp, int sample_rate) {
	classInitmydsp(sample_rate);
	instanceInitmydsp(dsp, sample_rate);
}

void buildUserInterfacemydsp(mydsp* dsp, UIGlue* ui_interface) {
	ui_interface->openVerticalBox(ui_interface->uiInterface, "M0SS-201-krell");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph3, "AMP", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "vbargraph0", &dsp->fVbargraph3, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f);
	ui_interface->openTabBox(ui_interface->uiInterface, "ui");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "main");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "signal path");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "oscillator");
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "bus");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider6, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider6, "midi", "13");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider6, "mod", "8");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "index", &dsp->fHslider6, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "alt-sel", "8");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "midi", "14");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "style", "radio{'AM':0;'FM':1;'TM':2}");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "type", &dsp->fHslider5, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "principal");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider4, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider4, "alt", "17");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider4, "midi", "15");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "tune", &dsp->fHslider4, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)-2.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "midi", "16");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "sel", "17");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "style", "radio{'SINE':0;'TRI':1;'BOTH':2}");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "waveform", &dsp->fHslider3, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox2, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox2, "alt-sel", "20");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox2, "midi", "17");
	ui_interface->addCheckButton(ui_interface->uiInterface, "quantize", &dsp->fCheckbox2);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fEntry0, "hidden", "1");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fEntry0, "pitch", "");
	ui_interface->addNumEntry(ui_interface->uiInterface, "freq", &dsp->fEntry0, (FAUSTFLOAT)6e+01f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)127.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fButton0, "gate", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fButton0, "hidden", "1");
	ui_interface->addButton(ui_interface->uiInterface, "gate", &dsp->fButton0);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "mixer");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider37, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider37, "midi", "20");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider37, "mod", "18");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "principal", &dsp->fVslider37, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider49, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider49, "midi", "21");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider49, "mod", "9");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "modulator", &dsp->fVslider49, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider50, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider50, "alt", "9");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider50, "midi", "22");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "stochastic", &dsp->fVslider50, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider48, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider48, "alt", "18");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider48, "midi", "23");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "envelope", &dsp->fVslider48, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "wavefolder");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider47, "midi", "19");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider47, "mod", "20");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "bias", &dsp->fVslider47, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)-0.5f, (FAUSTFLOAT)0.5f, (FAUSTFLOAT)0.01f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider51, "midi", "18");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider51, "mod", "23");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "timbre", &dsp->fVslider51, (FAUSTFLOAT)1.56f, (FAUSTFLOAT)0.7f, (FAUSTFLOAT)2.5f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "3", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "amplifier");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider3, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider3, "alt-sel", "19");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider3, "midi", "24");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider3, "style", "radio{'LPG':0;'VCA':1}");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mode", &dsp->fVslider3, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "alt", "19");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "midi", "25");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "volume", &dsp->fVslider0, (FAUSTFLOAT)0.4f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider32, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider32, "alt", "23");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider32, "midi", "26");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "glide", &dsp->fVslider32, (FAUSTFLOAT)0.001f, (FAUSTFLOAT)0.001f, (FAUSTFLOAT)0.99f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "4", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "delay");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider2, "alt", "25");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider2, "midi", "28");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "feedback", &dsp->fVslider2, (FAUSTFLOAT)0.647f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider1, "alt", "24");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider1, "midi", "29");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mix", &dsp->fVslider1, (FAUSTFLOAT)0.208f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.5f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider55, "alt", "16");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider55, "midi", "27");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "time", &dsp->fVslider55, (FAUSTFLOAT)6487.0f, (FAUSTFLOAT)1024.0f, (FAUSTFLOAT)8192.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "modulators");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "mod oscillator");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider1, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider1, "midi", "9");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider1, "mod", "2");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "course", &dsp->fHslider1, (FAUSTFLOAT)0.074f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider2, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider2, "alt", "2");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider2, "midi", "10");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "fine", &dsp->fHslider2, (FAUSTFLOAT)-0.017f, (FAUSTFLOAT)-0.03f, (FAUSTFLOAT)0.03f, (FAUSTFLOAT)0.0001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider0, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider0, "midi", "11");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider0, "sel", "6");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider0, "style", "radio{'TRI':0;'SQR':1;'SAW':2}");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "waveform", &dsp->fHslider0, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox0, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox0, "midi", "12");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox0, "sel", "4");
	ui_interface->addCheckButton(ui_interface->uiInterface, "LFO", &dsp->fCheckbox0);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox1, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox1, "alt-sel", "4");
	ui_interface->addCheckButton(ui_interface->uiInterface, "TRACK", &dsp->fCheckbox1);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph2, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph2, "MO", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "MO", &dsp->fVbargraph2, (FAUSTFLOAT)-1.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider6, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider6, "midi", "5");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider6, "sel", "11");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider6, "style", "radio{'SEPERATE':0;'SG':1;'ENV':2}");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "gate routing", &dsp->fVslider6, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "stochastic generator");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider11, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider11, "midi", "1");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider11, "mod", "1");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "length", &dsp->fVslider11, (FAUSTFLOAT)8.0f, (FAUSTFLOAT)4.0f, (FAUSTFLOAT)32.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider7, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider7, "midi", "2");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider7, "mod", "5");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "rate", &dsp->fVslider7, (FAUSTFLOAT)0.1f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider15, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider15, "midi", "3");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider15, "mod", "7");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "recall", &dsp->fVslider15, (FAUSTFLOAT)0.85f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.01f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "midi", "4");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "mod", "3");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "smooth", &dsp->fVslider19, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)0.25f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph0, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph0, "SG", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "SG", &dsp->fVbargraph0, (FAUSTFLOAT)-1.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "3", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "envelope");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider24, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider24, "midi", "6");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider24, "mod", "10");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "attack", &dsp->fVslider24, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider28, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider28, "midi", "7");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider28, "mod", "14");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "release", &dsp->fVslider28, (FAUSTFLOAT)0.625f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "midi", "8");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "sel", "12");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "style", "radio{'AR':0;'ASR':1;'LOOP':2}");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mode", &dsp->fVslider23, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph1, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph1, "LE", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "LE", &dsp->fVbargraph1, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "fb mod");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "from SG");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider13, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider13, "midi", "42");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_len", &dsp->fVslider13, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider53, "10", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider53, "midi", "52");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2fold", &dsp->fVslider53, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider5, "11", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider5, "midi", "53");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2amp", &dsp->fVslider5, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider9, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider9, "midi", "43");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_rate", &dsp->fVslider9, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider17, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider17, "midi", "44");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_recall", &dsp->fVslider17, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider21, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider21, "midi", "45");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_smooth", &dsp->fVslider21, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider25, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider25, "midi", "46");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2le_attack", &dsp->fVslider25, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider29, "5", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider29, "midi", "47");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2le_release", &dsp->fVslider29, (FAUSTFLOAT)0.362f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider34, "6", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider34, "midi", "48");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2mo_freq", &dsp->fVslider34, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider45, "7", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider45, "midi", "49");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2po_freq", &dsp->fVslider45, (FAUSTFLOAT)0.555f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider38, "8", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider38, "midi", "50");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2vca_ctrl", &dsp->fVslider38, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider43, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider43, "midi", "51");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2vca_input", &dsp->fVslider43, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "from LE");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider12, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider12, "midi", "30");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_len", &dsp->fVslider12, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider52, "10", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider52, "midi", "40");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2fold", &dsp->fVslider52, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider4, "11", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider4, "midi", "41");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2amp", &dsp->fVslider4, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider8, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider8, "midi", "31");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_rate", &dsp->fVslider8, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider16, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider16, "midi", "32");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_recall", &dsp->fVslider16, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider20, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider20, "midi", "33");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_smooth", &dsp->fVslider20, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider26, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider26, "midi", "34");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2le_attack", &dsp->fVslider26, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider30, "5", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider30, "midi", "35");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2le_release", &dsp->fVslider30, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider33, "6", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider33, "midi", "36");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2mo_freq", &dsp->fVslider33, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider44, "7", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider44, "midi", "37");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2po_freq", &dsp->fVslider44, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider39, "8", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider39, "midi", "38");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2vca_ctrl", &dsp->fVslider39, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider42, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider42, "midi", "39");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2vca_input", &dsp->fVslider42, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "from MO");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider14, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider14, "midi", "54");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_len", &dsp->fVslider14, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider54, "10", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider54, "midi", "64");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2fold", &dsp->fVslider54, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider36, "11", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider36, "midi", "65");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2amp", &dsp->fVslider36, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider10, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider10, "midi", "55");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_rate", &dsp->fVslider10, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider18, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider18, "midi", "56");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_recall", &dsp->fVslider18, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider22, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider22, "midi", "57");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_smooth", &dsp->fVslider22, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider27, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider27, "midi", "58");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2le_attack", &dsp->fVslider27, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider31, "5", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider31, "midi", "59");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2le_release", &dsp->fVslider31, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider35, "6", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider35, "midi", "60");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2mo_freq", &dsp->fVslider35, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider46, "7", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider46, "midi", "61");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2po_freq", &dsp->fVslider46, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider40, "8", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider40, "midi", "62");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2vca_ctrl", &dsp->fVslider40, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider41, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider41, "midi", "63");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2vca_input", &dsp->fVslider41, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
}

void computemydsp(mydsp* dsp, int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
	FAUSTFLOAT* output0 = outputs[0];
	FAUSTFLOAT* output1 = outputs[1];
	float fSlow0 = dsp->fConst1 * (float)(dsp->fVslider0);
	float fSlow1 = (float)(dsp->fVslider1);
	float fSlow2 = (float)(dsp->fVslider2);
	int iSlow3 = (int)((float)(dsp->fVslider3));
	float fSlow4 = (float)(dsp->fVslider4);
	int iSlow5 = fSlow4 == 0.0f;
	float fSlow6 = 0.5f * (float)(dsp->fVslider5);
	float fSlow7 = (float)(dsp->fVslider6);
	int iSlow8 = fSlow7 == 2.0f;
	float fSlow9 = 199.99f * mydsp_faustpower2_f((float)(dsp->fVslider7));
	float fSlow10 = (float)(dsp->fVslider8);
	float fSlow11 = (float)(dsp->fVslider9);
	float fSlow12 = (float)(dsp->fVslider10);
	int iSlow13 = (int)((float)(dsp->fVslider11));
	float fSlow14 = (float)(dsp->fVslider12);
	float fSlow15 = (float)(dsp->fVslider13);
	float fSlow16 = (float)(dsp->fVslider14);
	float fSlow17 = (float)(dsp->fVslider15);
	float fSlow18 = (float)(dsp->fVslider16);
	float fSlow19 = (float)(dsp->fVslider17);
	float fSlow20 = (float)(dsp->fVslider18);
	float fSlow21 = (float)(dsp->fVslider19);
	float fSlow22 = (float)(dsp->fVslider20);
	float fSlow23 = (float)(dsp->fVslider21);
	float fSlow24 = (float)(dsp->fVslider22);
	float fSlow25 = (float)(dsp->fVslider23);
	int iSlow26 = fSlow25 == 0.0f;
	int iSlow27 = fSlow25 == 1.0f;
	int iSlow28 = fSlow7 == 1.0f;
	float fSlow29 = (float)(dsp->fButton0);
	float fSlow30 = (float)(dsp->fVslider24);
	float fSlow31 = (float)(dsp->fVslider25);
	float fSlow32 = (float)(dsp->fVslider26);
	float fSlow33 = (float)(dsp->fVslider27);
	float fSlow34 = (float)(dsp->fVslider28);
	float fSlow35 = (float)(dsp->fVslider29);
	float fSlow36 = (float)(dsp->fVslider30);
	float fSlow37 = (float)(dsp->fVslider31);
	int iSlow38 = fSlow25 == 2.0f;
	float fSlow39 = (float)(dsp->fHslider0);
	int iSlow40 = fSlow39 == 0.0f;
	int iSlow41 = fSlow39 == 1.0f;
	float fSlow42 = (float)(dsp->fCheckbox0);
	int iSlow43 = fSlow42 == 1.0f;
	int iSlow44 = (int)(fSlow42);
	int iSlow45 = (int)((float)(dsp->fCheckbox1));
	float fSlow46 = fmaxf(0.0f, fminf(1.0f, (float)(dsp->fHslider1) + (float)(dsp->fHslider2)));
	float fSlow47 = 2.98e+03f * mydsp_faustpower2_f(fSlow46) + 2e+01f;
	float fSlow48 = powf(2.0f, 4.0f * fSlow46 + -2.0f);
	float fSlow49 = (float)(dsp->fVslider32);
	int iSlow50 = fabsf(fSlow49) < 1.1920929e-07f;
	float fSlow51 = ((iSlow50) ? 0.0f : expf(-(dsp->fConst4 / ((iSlow50) ? 1.0f : fSlow49))));
	float fSlow52 = 4.4e+02f * powf(2.0f, 0.083333336f * ((float)(dsp->fEntry0) + -69.0f)) * (1.0f - fSlow51);
	float fSlow53 = (float)(dsp->fVslider33);
	float fSlow54 = (float)(dsp->fVslider34);
	float fSlow55 = (float)(dsp->fVslider35);
	float fSlow56 = (float)(dsp->fVslider36);
	float fSlow57 = (float)(dsp->fVslider37);
	float fSlow58 = (float)(dsp->fHslider3);
	int iSlow59 = fSlow58 == 0.0f;
	int iSlow60 = fSlow58 == 1.0f;
	int iSlow61 = (int)((float)(dsp->fCheckbox2));
	float fSlow62 = powf(2.0f, (float)(dsp->fHslider4));
	float fSlow63 = (float)(dsp->fHslider5);
	int iSlow64 = fSlow63 == 1.0f;
	float fSlow65 = (float)(dsp->fHslider6);
	float fSlow66 = (float)(dsp->fVslider38);
	float fSlow67 = (float)(dsp->fVslider39);
	float fSlow68 = (float)(dsp->fVslider40);
	float fSlow69 = (float)(dsp->fVslider41);
	float fSlow70 = (float)(dsp->fVslider42);
	float fSlow71 = (float)(dsp->fVslider43);
	float fSlow72 = 2.0f * (float)(dsp->fVslider44);
	float fSlow73 = 2.0f * (float)(dsp->fVslider45);
	float fSlow74 = 2.0f * (float)(dsp->fVslider46);
	float fSlow75 = 0.0022727272f * fSlow62;
	int iSlow76 = fSlow63 == 0.0f;
	float fSlow77 = dsp->fConst1 * (float)(dsp->fVslider47);
	float fSlow78 = (float)(dsp->fVslider48);
	float fSlow79 = (float)(dsp->fVslider49);
	float fSlow80 = (float)(dsp->fVslider50);
	float fSlow81 = dsp->fConst1 * (float)(dsp->fVslider51);
	int iSlow82 = fSlow63 == 2.0f;
	float fSlow83 = (float)(dsp->fVslider52);
	float fSlow84 = 0.5f * (float)(dsp->fVslider53);
	float fSlow85 = (float)(dsp->fVslider54);
	float fSlow86 = (float)(dsp->fVslider55);
	float fSlow87 = 1.0f - fSlow1;
	/* C99 loop */
	{
		int i0;
		for (i0 = 0; i0 < count; i0 = i0 + 1) {
			dsp->iVec0[0] = 1;
			dsp->fRec0[0] = fSlow0 + dsp->fConst2 * dsp->fRec0[1];
			int iTemp0 = 1 - dsp->iVec0[1];
			float fTemp1 = fSlow9 + 199.9f * mydsp_faustpower2_f(fSlow10 * dsp->fRec5[1] + fSlow11 * dsp->fRec3[1] + fSlow12 * dsp->fRec7[1]) + 0.11f;
			float fTemp2 = ((iTemp0) ? 0.0f : dsp->fRec11[1] + dsp->fConst4 * fTemp1);
			dsp->fRec11[0] = fTemp2 - floorf(fTemp2);
			float fTemp3 = ((iSlow8) ? ((iSlow8) ? dsp->fRec6[1] : 0.0f) : (float)(dsp->fRec11[0] <= 0.5f));
			dsp->fVec1[0] = fTemp3;
			dsp->iRec10[0] = dsp->iRec10[1] + (fTemp3 > dsp->fVec1[1]);
			int iTemp4 = max(1, min(32, iSlow13 + (int)(32.0f * (fSlow14 * dsp->fRec5[1] + fSlow15 * dsp->fRec3[1] + fSlow16 * dsp->fRec7[1])))) + 1;
			int iTemp5 = dsp->iRec10[0] % iTemp4;
			int iTemp6 = (dsp->fVec1[1] <= 0.0f) & (fTemp3 > 0.0f);
			float fTemp7 = (float)(1 - iTemp6);
			int iTemp8 = 1103515245 * (dsp->iRec13[1] + 12345);
			dsp->iRec13[0] = 1103515245 * (iTemp8 + 12345);
			int iRec14 = iTemp8;
			float fTemp9 = (float)(iTemp6);
			dsp->fRec12[0] = dsp->fRec12[1] * fTemp7 + 4.656613e-10f * (float)(iRec14) * fTemp9;
			dsp->fRec15[0] = fTemp7 * dsp->fRec15[1] + 4.656613e-10f * (float)(dsp->iRec13[0]) * fTemp9;
			dsp->ftbl0[max(0, min(iTemp5, 31))] = (((0.5f * (dsp->fRec12[0] + 1.0f)) > fmaxf(0.0f, fminf(1.0f, fSlow17 + fSlow18 * dsp->fRec5[1] + fSlow19 * dsp->fRec3[1] + fSlow20 * dsp->fRec7[1]))) ? dsp->fRec15[0] : dsp->fRec9[1]);
			dsp->fRec9[0] = dsp->ftbl0[max(0, min((iTemp5 + 1) % iTemp4, 31))];
			float fTemp10 = fmaxf(0.0f, fminf(1.0f, fSlow21 + fSlow22 * dsp->fRec5[1] + fSlow23 * dsp->fRec3[1] + fSlow24 * dsp->fRec7[1])) / fTemp1;
			int iTemp11 = fabsf(fTemp10) < 1.1920929e-07f;
			float fTemp12 = ((iTemp11) ? 0.0f : expf(-(dsp->fConst4 / ((iTemp11) ? 1.0f : fTemp10))));
			dsp->fRec8[0] = dsp->fRec9[0] * (1.0f - fTemp12) + fTemp12 * dsp->fRec8[1];
			dsp->fRec3[0] = dsp->fRec8[0];
			dsp->fRec4[0] = fTemp3;
			float fTemp13 = ((iSlow28) ? dsp->fRec4[1] : fSlow29);
			dsp->fVec2[0] = fTemp13;
			float fTemp14 = fTemp13 - dsp->fVec2[1];
			int iTemp15 = (int)(fTemp14 * (float)(fTemp14 > 0.0f));
			int iTemp16 = ((iTemp15) ? 0 : ((dsp->iRec16[1]) ? ((dsp->fRec17[1] > 0.0f) ? 1 : 0) : ((dsp->fRec17[1] < 1.0f) ? 0 : 1)));
			dsp->iRec16[0] = iTemp16;
			float fTemp17 = 1.0f / fmaxf(1.0f, dsp->fConst0 * (fSlow30 + fSlow31 * dsp->fRec3[1] + fSlow32 * dsp->fRec5[1] + fSlow33 * dsp->fRec7[1]));
			float fTemp18 = fmaxf(1.0f, dsp->fConst0 * (fSlow34 + fSlow35 * dsp->fRec3[1] + fSlow36 * dsp->fRec5[1] + fSlow37 * dsp->fRec7[1]));
			float fTemp19 = 1.0f / fTemp18;
			float fTemp20 = fmaxf(0.03f, fminf(0.97f, ((dsp->iRec16[1]) ? 0.3f : 0.7f)));
			float fTemp21 = fTemp20 + -1.0f;
			float fTemp22 = dsp->fRec17[1] * fTemp21 / (fTemp20 * (2.0f * dsp->fRec17[1] + -1.0f) - dsp->fRec17[1]);
			dsp->fRec17[0] = ((iTemp15) ? 0.0f : fmaxf(0.0f, fminf(1.0f, dsp->fRec17[1] - ((dsp->iRec16[1]) ? -fTemp19 : fTemp17) * fTemp20 * fTemp21 / mydsp_faustpower2_f(fTemp20 * (2.0f * fTemp22 + -1.0f) + (1.0f - fTemp22)))));
			int iRec18 = ((((dsp->iRec16[1] == (1 & iTemp16)) == 0) | iTemp15) ? 1 : 0);
			int iTemp23 = fTemp13 > 0.0f;
			int iTemp24 = dsp->iRec19[1] == 0;
			int iTemp25 = dsp->iRec19[1] == 1;
			dsp->iRec21[0] = dsp->iRec21[1] + 1;
			int iTemp26 = (dsp->iRec21[0] + -1) == 0;
			float fTemp27 = ((iTemp26) ? 0.0f : dsp->fRec20[1]);
			dsp->iRec19[0] = ((iTemp23) ? ((iTemp24) ? 1 : ((iTemp25) ? ((fTemp27 < 1.0f) ? 1 : 2) : 2)) : 0);
			float fTemp28 = (float)(((dsp->iRec19[1] == 2) ? 1 : 0));
			int iTemp29 = fTemp13 == 0.0f;
			dsp->iVec3[0] = iTemp29;
			int iTemp30 = (dsp->iVec3[1] <= 0) & (iTemp29 > 0);
			float fTemp31 = (float)(1 - iTemp30);
			float fTemp32 = (float)(iTemp30);
			dsp->fRec22[0] = dsp->fRec22[1] * fTemp31 + ((iTemp26) ? 1.0f : fTemp27) * fTemp32;
			float fTemp33 = fmaxf(0.03f, fminf(0.97f, ((iTemp24) ? 0.3f : ((iTemp25) ? 0.7f : 0.3f))));
			float fTemp34 = fTemp33 + -1.0f;
			float fTemp35 = ((iTemp24) ? dsp->fRec22[0] : 1.0f);
			float fTemp36 = ((fTemp28 == fTemp35) ? 0.5f : (fTemp27 - fTemp28) / (fTemp35 - fTemp28));
			float fTemp37 = fTemp34 * fTemp36 / (fTemp33 * (2.0f * fTemp36 + -1.0f) - fTemp36);
			dsp->fRec20[0] = fmaxf(fTemp28, fminf(1.0f, fTemp27 - ((iTemp24) ? -(dsp->fRec22[0] / fTemp18) : ((iTemp25) ? fTemp17 : 0.0f)) * fTemp33 * fTemp34 / mydsp_faustpower2_f(fTemp33 * (2.0f * fTemp37 + -1.0f) + (1.0f - fTemp37))));
			int iTemp38 = dsp->iRec23[1] == 0;
			int iTemp39 = dsp->iRec23[1] == 1;
			float fTemp40 = ((iTemp26) ? 0.0f : dsp->fRec24[1]);
			dsp->iRec23[0] = ((iTemp23) ? ((iTemp38) ? 1 : ((iTemp39) ? ((fTemp40 < 1.0f) ? 1 : 2) : 2)) : 0);
			dsp->fRec25[0] = fTemp31 * dsp->fRec25[1] + fTemp32 * ((iTemp26) ? 1.0f : fTemp40);
			float fTemp41 = fmaxf(0.03f, fminf(0.97f, ((iTemp38) ? 0.3f : ((iTemp39) ? 0.7f : 0.3f))));
			float fTemp42 = fTemp41 + -1.0f;
			float fTemp43 = ((iTemp38) ? dsp->fRec25[0] : 1.0f);
			float fTemp44 = ((0.0f == fTemp43) ? 0.5f : fTemp40 / fTemp43);
			float fTemp45 = fTemp42 * fTemp44 / (fTemp41 * (2.0f * fTemp44 + -1.0f) - fTemp44);
			dsp->fRec24[0] = fmaxf(0.0f, fminf(1.0f, fTemp40 - ((iTemp38) ? -(dsp->fRec25[0] / fTemp18) : ((iTemp39) ? fTemp17 : -fTemp19)) * fTemp41 * fTemp42 / mydsp_faustpower2_f(fTemp41 * (2.0f * fTemp45 + -1.0f) + (1.0f - fTemp45))));
			dsp->fRec5[0] = ((iSlow26) ? dsp->fRec24[0] : ((iSlow27) ? dsp->fRec20[0] : dsp->fRec17[0]));
			dsp->fRec6[0] = ((iSlow38) ? (float)(iRec18) : fTemp13);
			dsp->fRec28[0] = fSlow52 + fSlow51 * dsp->fRec28[1];
			float fTemp46 = ((iSlow45) ? fSlow48 * dsp->fRec28[0] : fSlow47);
			float fTemp47 = ((iSlow44) ? 0.1f * fTemp46 : fTemp46);
			float fTemp48 = powf(2.0f, 2.0f * (fSlow53 * dsp->fRec5[1] + fSlow54 * dsp->fRec3[1] + fSlow55 * dsp->fRec7[1]));
			float fTemp49 = fTemp47 * fTemp48;
			float fTemp50 = fmaxf(1.1920929e-07f, fabsf(fTemp49));
			float fTemp51 = dsp->fRec26[1] + dsp->fConst4 * fTemp50;
			float fTemp52 = fTemp51 + -1.0f;
			int iTemp53 = fTemp52 < 0.0f;
			dsp->fRec26[0] = ((iTemp53) ? fTemp51 : fTemp52);
			float fRec27 = ((iTemp53) ? fTemp51 : fTemp51 + fTemp52 * (1.0f - dsp->fConst0 / fTemp50));
			float fTemp54 = ((iTemp0) ? 0.0f : dsp->fRec29[1] + dsp->fConst4 * fTemp49);
			dsp->fRec29[0] = fTemp54 - floorf(fTemp54);
			float fTemp55 = 2.0f * dsp->fRec29[0] + -1.0f;
			float fTemp56 = (float)(dsp->iVec0[1]);
			float fTemp57 = fmaxf(fTemp49, 23.44895f);
			float fTemp58 = fmaxf(2e+01f, fabsf(fTemp57));
			float fTemp59 = ((iTemp0) ? 0.0f : dsp->fRec30[1] + dsp->fConst4 * fTemp58);
			dsp->fRec30[0] = fTemp59 - floorf(fTemp59);
			float fTemp60 = mydsp_faustpower2_f(2.0f * dsp->fRec30[0] + -1.0f);
			dsp->fVec4[0] = fTemp60;
			float fTemp61 = fTemp56 * (fTemp60 - dsp->fVec4[1]) / fTemp58;
			dsp->fVec5[dsp->IOTA0 & 4095] = fTemp61;
			float fTemp62 = fmaxf(0.0f, fminf(2047.0f, dsp->fConst6 / fTemp57));
			int iTemp63 = (int)(fTemp62);
			float fTemp64 = floorf(fTemp62);
			float fTemp65 = dsp->fConst5 * (fTemp61 - dsp->fVec5[(dsp->IOTA0 - iTemp63) & 4095] * (fTemp64 + (1.0f - fTemp62)) - (fTemp62 - fTemp64) * dsp->fVec5[(dsp->IOTA0 - (iTemp63 + 1)) & 4095]);
			dsp->fRec31[0] = fTemp65 + 0.999f * dsp->fRec31[1];
			dsp->fRec7[0] = ((iSlow40) ? ((iSlow43) ? 2.0f * (1.0f - fabsf(fTemp55)) + -1.0f : dsp->fConst7 * dsp->fRec31[0] * fTemp47 * fTemp48) : ((iSlow41) ? ((iSlow43) ? 2.0f * (float)(dsp->fRec29[0] <= 0.5f) + -1.0f : fTemp65) : ((iSlow43) ? fTemp55 : 2.0f * fRec27 + -1.0f)));
			dsp->fVbargraph0 = (FAUSTFLOAT)(dsp->fRec3[0]);
			float fTemp66 = dsp->fRec3[0];
			float fTemp67 = fTemp66 + 1.0f;
			dsp->fVbargraph1 = (FAUSTFLOAT)(dsp->fRec5[0]);
			float fTemp68 = dsp->fRec5[0];
			dsp->fVbargraph2 = (FAUSTFLOAT)(dsp->fRec7[0]);
			float fTemp69 = dsp->fRec7[0];
			float fTemp70 = ((iSlow5) ? 1.0f : fSlow6 * fTemp67 + fSlow4 * fTemp68 + fSlow56 * fTemp69);
			float fTemp71 = 1.0f / tanf(dsp->fConst3 * (mydsp_faustpower2_f(9e+01f * fTemp70) + 0.01f));
			float fTemp72 = fmaxf(0.0f, fminf(1.0f, fSlow65 + 0.5f * (fSlow66 * fTemp66 + fSlow67 * fTemp68 + fSlow68 * fTemp69)));
			float fTemp73 = fSlow69 * fTemp69 + fSlow70 * fTemp68 + fSlow71 * fTemp66;
			float fTemp74 = fTemp72 * fTemp73;
			float fTemp75 = dsp->fRec28[0] * ((iSlow64) ? powf(2.0f, 2.0f * fTemp74) : 1.0f) * powf(2.0f, fSlow72 * fTemp68) * powf(2.0f, fSlow73 * fTemp66) * powf(2.0f, fSlow74 * fTemp69);
			float fTemp76 = fSlow62 * fTemp75;
			float fTemp77 = 0.051175587f * (fTemp76 + -1e+01f);
			int iTemp78 = (int)(fTemp77);
			float fTemp79 = ftbl2mydspSIG2[max(0, min(iTemp78, 1023))];
			float fTemp80 = fTemp77 - (float)(iTemp78);
			float fTemp81 = ftbl2mydspSIG2[max(0, min(iTemp78 + 1, 1023))];
			float fTemp82 = ftbl2mydspSIG2[max(0, min(iTemp78 + -1, 1023))];
			float fTemp83 = ftbl2mydspSIG2[max(0, min(iTemp78 + 2, 1023))];
			float fTemp84 = 1.442695f * (fTemp79 + 0.5f * fTemp80 * (fTemp81 + fTemp80 * (2.0f * fTemp82 + 4.0f * fTemp81 + fTemp80 * (fTemp83 + 3.0f * (fTemp79 - fTemp81) - fTemp82) - (5.0f * fTemp79 + fTemp83)) - fTemp82) + -6.086775f);
			float fTemp85 = (float)(fTemp84 < 0.0f) / (float)(1 << (int)(-(fTemp84 + -1.0f))) + (float)((fTemp84 >= 0.0f) * (1 << (int)(fTemp84)));
			float fTemp86 = fSlow75 * (fTemp75 / fTemp85);
			float fTemp87 = ((iSlow61) ? 4.4e+02f * fTemp85 * (1.7817974f * (float)(1.7817974f <= fTemp86) + (float)(1.7817974f > fTemp86) * (1.4983071f * (float)(1.4983071f <= fTemp86) + (float)(1.4983071f > fTemp86) * (1.3348398f * (float)(1.3348398f <= fTemp86) + (float)(1.3348398f > fTemp86) * (1.1892071f * (float)(1.1892071f <= fTemp86) + (float)(1.1892071f > fTemp86))))) : fTemp76);
			float fTemp88 = ((iTemp0) ? 0.0f : dsp->fRec34[1] + dsp->fConst4 * fTemp87);
			dsp->fRec34[0] = fTemp88 - floorf(fTemp88);
			float fTemp89 = ftbl1mydspSIG1[max(0, min((int)(65536.0f * dsp->fRec34[0]), 65535))];
			float fTemp90 = fmaxf(fTemp87, 23.44895f);
			float fTemp91 = fmaxf(2e+01f, fabsf(fTemp90));
			float fTemp92 = ((iTemp0) ? 0.0f : dsp->fRec37[1] + dsp->fConst4 * fTemp91);
			dsp->fRec37[0] = fTemp92 - floorf(fTemp92);
			float fTemp93 = mydsp_faustpower2_f(2.0f * dsp->fRec37[0] + -1.0f);
			dsp->fVec7[0] = fTemp93;
			float fTemp94 = fTemp56 * (fTemp93 - dsp->fVec7[1]) / fTemp91;
			dsp->fVec8[dsp->IOTA0 & 4095] = fTemp94;
			float fTemp95 = fmaxf(0.0f, fminf(2047.0f, dsp->fConst6 / fTemp90));
			int iTemp96 = (int)(fTemp95);
			float fTemp97 = floorf(fTemp95);
			dsp->fRec36[0] = 0.999f * dsp->fRec36[1] + dsp->fConst5 * (fTemp94 - dsp->fVec8[(dsp->IOTA0 - iTemp96) & 4095] * (fTemp97 + (1.0f - fTemp95)) - (fTemp95 - fTemp97) * dsp->fVec8[(dsp->IOTA0 - (iTemp96 + 1)) & 4095]);
			float fTemp98 = dsp->fConst7 * dsp->fRec36[0] * fTemp87;
			dsp->fRec38[0] = fSlow77 + dsp->fConst2 * dsp->fRec38[1];
			dsp->fRec39[0] = fSlow81 + dsp->fConst2 * dsp->fRec39[1];
			float fTemp99 = mydsp_faustpower2_f(fmaxf(0.0f, fminf(3.0f, dsp->fRec39[0] + ((iSlow82) ? fTemp72 * (0.5f * fTemp73 + 1.0f) : 0.0f) + fSlow83 * fTemp68 + fSlow84 * fTemp67 + fSlow85 * fTemp69)));
			float fTemp100 = sinf(1.5707964f * (fSlow57 * ((iSlow59) ? fTemp89 : ((iSlow60) ? fTemp98 : 0.5f * (fTemp89 + fTemp98))) * ((iSlow76) ? fTemp74 + 1.0f : 1.0f) + dsp->fRec38[0] + fSlow78 * fTemp68 + fSlow79 * fTemp69 + fSlow80 * fTemp66) * fTemp99) * ((fTemp99 < 1.0f) ? 1.0f / sinf(1.5707964f * fTemp99) : 1.0f);
			dsp->fVec9[0] = fTemp100;
			dsp->fRec32[0] = 0.995f * dsp->fRec32[1] + fTemp100 - dsp->fVec9[1];
			dsp->fRec2[0] = -((dsp->fRec2[1] * (1.0f - fTemp71) - (dsp->fRec32[0] + dsp->fRec32[1])) / (fTemp71 + 1.0f));
			float fTemp101 = ((iSlow3) ? dsp->fRec32[0] * fTemp70 : dsp->fRec2[0] * fTemp70);
			dsp->fVbargraph3 = (FAUSTFLOAT)(fTemp101);
			float fTemp102 = fTemp101;
			float fTemp103 = fSlow2 * dsp->fRec1[1] + fTemp102;
			dsp->fVec10[dsp->IOTA0 & 16383] = fTemp103;
			float fTemp104 = ((dsp->fRec40[1] != 0.0f) ? (((dsp->fRec41[1] > 0.0f) & (dsp->fRec41[1] < 1.0f)) ? dsp->fRec40[1] : 0.0f) : (((dsp->fRec41[1] == 0.0f) & (fSlow86 != dsp->fRec42[1])) ? dsp->fConst8 : (((dsp->fRec41[1] == 1.0f) & (fSlow86 != dsp->fRec43[1])) ? -dsp->fConst8 : 0.0f)));
			dsp->fRec40[0] = fTemp104;
			dsp->fRec41[0] = fmaxf(0.0f, fminf(1.0f, dsp->fRec41[1] + fTemp104));
			dsp->fRec42[0] = (((dsp->fRec41[1] >= 1.0f) & (dsp->fRec43[1] != fSlow86)) ? fSlow86 : dsp->fRec42[1]);
			dsp->fRec43[0] = (((dsp->fRec41[1] <= 0.0f) & (dsp->fRec42[1] != fSlow86)) ? fSlow86 : dsp->fRec43[1]);
			float fTemp105 = dsp->fVec10[(dsp->IOTA0 - (int)(fminf(8192.0f, fmaxf(0.0f, dsp->fRec42[0])))) & 16383];
			dsp->fRec1[0] = fTemp105 + dsp->fRec41[0] * (dsp->fVec10[(dsp->IOTA0 - (int)(fminf(8192.0f, fmaxf(0.0f, dsp->fRec43[0])))) & 16383] - fTemp105);
			float fTemp106 = dsp->fRec0[0] * (fSlow1 * dsp->fRec1[0] + fSlow87 * fTemp102);
			output0[i0] = (FAUSTFLOAT)(fTemp106);
			output1[i0] = (FAUSTFLOAT)(fTemp106);
			dsp->iVec0[1] = dsp->iVec0[0];
			dsp->fRec0[1] = dsp->fRec0[0];
			dsp->fRec11[1] = dsp->fRec11[0];
			dsp->fVec1[1] = dsp->fVec1[0];
			dsp->iRec10[1] = dsp->iRec10[0];
			dsp->iRec13[1] = dsp->iRec13[0];
			dsp->fRec12[1] = dsp->fRec12[0];
			dsp->fRec15[1] = dsp->fRec15[0];
			dsp->fRec9[1] = dsp->fRec9[0];
			dsp->fRec8[1] = dsp->fRec8[0];
			dsp->fRec3[1] = dsp->fRec3[0];
			dsp->fRec4[1] = dsp->fRec4[0];
			dsp->fVec2[1] = dsp->fVec2[0];
			dsp->iRec16[1] = dsp->iRec16[0];
			dsp->fRec17[1] = dsp->fRec17[0];
			dsp->iRec21[1] = dsp->iRec21[0];
			dsp->iRec19[1] = dsp->iRec19[0];
			dsp->iVec3[1] = dsp->iVec3[0];
			dsp->fRec22[1] = dsp->fRec22[0];
			dsp->fRec20[1] = dsp->fRec20[0];
			dsp->iRec23[1] = dsp->iRec23[0];
			dsp->fRec25[1] = dsp->fRec25[0];
			dsp->fRec24[1] = dsp->fRec24[0];
			dsp->fRec5[1] = dsp->fRec5[0];
			dsp->fRec6[1] = dsp->fRec6[0];
			dsp->fRec28[1] = dsp->fRec28[0];
			dsp->fRec26[1] = dsp->fRec26[0];
			dsp->fRec29[1] = dsp->fRec29[0];
			dsp->fRec30[1] = dsp->fRec30[0];
			dsp->fVec4[1] = dsp->fVec4[0];
			dsp->IOTA0 = dsp->IOTA0 + 1;
			dsp->fRec31[1] = dsp->fRec31[0];
			dsp->fRec7[1] = dsp->fRec7[0];
			dsp->fRec34[1] = dsp->fRec34[0];
			dsp->fRec37[1] = dsp->fRec37[0];
			dsp->fVec7[1] = dsp->fVec7[0];
			dsp->fRec36[1] = dsp->fRec36[0];
			dsp->fRec38[1] = dsp->fRec38[0];
			dsp->fRec39[1] = dsp->fRec39[0];
			dsp->fVec9[1] = dsp->fVec9[0];
			dsp->fRec32[1] = dsp->fRec32[0];
			dsp->fRec2[1] = dsp->fRec2[0];
			dsp->fRec40[1] = dsp->fRec40[0];
			dsp->fRec41[1] = dsp->fRec41[0];
			dsp->fRec42[1] = dsp->fRec42[0];
			dsp->fRec43[1] = dsp->fRec43[0];
			dsp->fRec1[1] = dsp->fRec1[0];
		}
	}
}

#ifdef __cplusplus
}
#endif

#endif
