/* ------------------------------------------------------------
name: "M0SS-201"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -lang c -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
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
	int iVec8[2];
	int iRec44[2];
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
		int l39;
		for (l39 = 0; l39 < 2; l39 = l39 + 1) {
			dsp->iVec8[l39] = 0;
		}
	}
	/* C99 loop */
	{
		int l40;
		for (l40 = 0; l40 < 2; l40 = l40 + 1) {
			dsp->iRec44[l40] = 0;
		}
	}
}

static void fillmydspSIG1(mydspSIG1* dsp, int count, float* table) {
	/* C99 loop */
	{
		int i2;
		for (i2 = 0; i2 < count; i2 = i2 + 1) {
			dsp->iVec8[0] = 1;
			dsp->iRec44[0] = (dsp->iVec8[1] + dsp->iRec44[1]) % 65536;
			table[i2] = sinf(9.58738e-05f * (float)(dsp->iRec44[0]));
			dsp->iVec8[1] = dsp->iVec8[0];
			dsp->iRec44[1] = dsp->iRec44[0];
		}
	}
}

static float mydsp_faustpower2_f(float value) {
	return value * value;
}
static float ftbl1mydspSIG1[65536];

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif
static inline int max_i(int a, int b) { return (a > b) ? a : b; }
static inline int min_i(int a, int b) { return (a < b) ? a : b; }

typedef struct {
	FAUSTFLOAT fVslider0;
	FAUSTFLOAT fVslider1;
	int iVec0[2];
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	FAUSTFLOAT fVslider7;
	FAUSTFLOAT fVslider8;
	FAUSTFLOAT fVslider9;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float ftbl0[32];
	FAUSTFLOAT fVslider10;
	FAUSTFLOAT fVslider11;
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fVslider13;
	FAUSTFLOAT fVslider14;
	float fRec9[2];
	float fVec1[2];
	int iRec10[2];
	float fRec8[2];
	float fRec12[2];
	FAUSTFLOAT fVslider15;
	FAUSTFLOAT fVslider16;
	FAUSTFLOAT fVslider17;
	FAUSTFLOAT fVslider18;
	int iRec13[2];
	float fRec7[2];
	float fRec6[2];
	float fRec1[2];
	float fRec2[2];
	FAUSTFLOAT fVslider19;
	FAUSTFLOAT fButton0;
	float fVec2[2];
	int iRec14[2];
	FAUSTFLOAT fVslider20;
	FAUSTFLOAT fVslider21;
	FAUSTFLOAT fVslider22;
	FAUSTFLOAT fVslider23;
	FAUSTFLOAT fVslider24;
	FAUSTFLOAT fVslider25;
	FAUSTFLOAT fVslider26;
	FAUSTFLOAT fVslider27;
	float fRec15[2];
	int iRec19[2];
	int iRec17[2];
	int iVec3[2];
	float fRec20[2];
	float fRec18[2];
	int iRec21[2];
	float fRec23[2];
	float fRec22[2];
	float fRec3[2];
	float fRec4[2];
	FAUSTFLOAT fCheckbox0;
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fVslider28;
	FAUSTFLOAT fVslider29;
	FAUSTFLOAT fVslider30;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fVslider31;
	FAUSTFLOAT fEntry0;
	float fRec26[2];
	float fRec24[2];
	float fRec27[2];
	float fRec28[2];
	float fVec4[2];
	int IOTA0;
	float fVec5[4096];
	float fConst2;
	float fConst3;
	float fRec29[2];
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fRec30[3];
	float fRec5[2];
	FAUSTFLOAT fVbargraph0;
	FAUSTFLOAT fVslider32;
	FAUSTFLOAT fVbargraph1;
	FAUSTFLOAT fVslider33;
	FAUSTFLOAT fVbargraph2;
	float fConst10;
	FAUSTFLOAT fHslider3;
	FAUSTFLOAT fVslider34;
	FAUSTFLOAT fVslider35;
	FAUSTFLOAT fVslider36;
	FAUSTFLOAT fVslider37;
	FAUSTFLOAT fVslider38;
	FAUSTFLOAT fVslider39;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fVslider40;
	FAUSTFLOAT fVslider41;
	FAUSTFLOAT fVslider42;
	float fConst11;
	float fConst12;
	FAUSTFLOAT fVslider43;
	float fRec40[2];
	FAUSTFLOAT fVslider44;
	FAUSTFLOAT fVslider45;
	FAUSTFLOAT fVslider46;
	FAUSTFLOAT fVslider47;
	float fRec41[2];
	FAUSTFLOAT fHslider5;
	FAUSTFLOAT fCheckbox2;
	FAUSTFLOAT fVslider48;
	FAUSTFLOAT fVslider49;
	FAUSTFLOAT fVslider50;
	FAUSTFLOAT fHslider6;
	float fRec43[2];
	float fVec6[2];
	float fVec7[4096];
	float fRec42[2];
	float fRec45[2];
	float fVec9[3];
	FAUSTFLOAT fVslider51;
	float fVec10[2];
	float fConst13;
	float fConst14;
	float fConst15;
	float fRec39[2];
	float fVec11[2];
	float fRec38[2];
	float fVec12[2];
	float fRec37[2];
	float fVec13[2];
	float fRec36[2];
	float fVec14[2];
	float fRec35[2];
	float fVec15[2];
	float fRec34[2];
	float fVec16[2];
	float fRec33[2];
	float fVec17[2];
	float fRec32[2];
	float fRec31[2];
	FAUSTFLOAT fVbargraph3;
	float fVec18[2];
	float fRec0[2];
	FAUSTFLOAT fVslider52;
	FAUSTFLOAT fVslider53;
	float fVec19[16384];
	FAUSTFLOAT fVslider54;
	float fConst16;
	float fRec47[2];
	float fRec48[2];
	float fRec49[2];
	float fRec50[2];
	float fRec46[2];
	FAUSTFLOAT fVslider55;
	float fRec51[2];
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
	m->declare(m->metaInterface, "aanl.lib/version", "1.4.2");
	m->declare(m->metaInterface, "basics.lib/counter:author", "Stephane Letz");
	m->declare(m->metaInterface, "basics.lib/name", "Faust Basic Element Library");
	m->declare(m->metaInterface, "basics.lib/version", "1.22.0");
	m->declare(m->metaInterface, "compile_options", "-lang c -fpga-mem-th 4 -ct 1 -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
	m->declare(m->metaInterface, "delays.lib/name", "Faust Delay Library");
	m->declare(m->metaInterface, "delays.lib/version", "1.2.0");
	m->declare(m->metaInterface, "envelopes.lib/adsr_bias:author", "Andrew John March and David Braun");
	m->declare(m->metaInterface, "envelopes.lib/adsr_bias:licence", "STK-4.3");
	m->declare(m->metaInterface, "envelopes.lib/author", "GRAME");
	m->declare(m->metaInterface, "envelopes.lib/copyright", "GRAME");
	m->declare(m->metaInterface, "envelopes.lib/license", "LGPL with exception");
	m->declare(m->metaInterface, "envelopes.lib/name", "Faust Envelope Library");
	m->declare(m->metaInterface, "envelopes.lib/version", "1.3.0");
	m->declare(m->metaInterface, "filename", "M0SS-201.dsp");
	m->declare(m->metaInterface, "filters.lib/dcblocker:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/fir:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/fir:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/iir:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/iir:license", "MIT-style STK-4.3 license");
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
	m->declare(m->metaInterface, "filters.lib/tf2:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/tf2:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/tf2s:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/tf2s:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "filters.lib/version", "1.7.1");
	m->declare(m->metaInterface, "filters.lib/zero:author", "Julius O. Smith III");
	m->declare(m->metaInterface, "filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
	m->declare(m->metaInterface, "filters.lib/zero:license", "MIT-style STK-4.3 license");
	m->declare(m->metaInterface, "interpolators.lib/name", "Faust Interpolator Library");
	m->declare(m->metaInterface, "interpolators.lib/remap:author", "David Braun");
	m->declare(m->metaInterface, "interpolators.lib/version", "1.4.0");
	m->declare(m->metaInterface, "maths.lib/author", "GRAME");
	m->declare(m->metaInterface, "maths.lib/copyright", "GRAME");
	m->declare(m->metaInterface, "maths.lib/license", "LGPL with exception");
	m->declare(m->metaInterface, "maths.lib/name", "Faust Math Library");
	m->declare(m->metaInterface, "maths.lib/version", "2.9.0");
	m->declare(m->metaInterface, "name", "M0SS-201");
	m->declare(m->metaInterface, "noises.lib/name", "Faust Noise Generator Library");
	m->declare(m->metaInterface, "noises.lib/version", "1.5.0");
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
	m->declare(m->metaInterface, "oscillators.lib/version", "1.6.0");
	m->declare(m->metaInterface, "platform.lib/name", "Generic Platform Library");
	m->declare(m->metaInterface, "platform.lib/version", "1.3.0");
	m->declare(m->metaInterface, "quantizers.lib/name", "Faust Frequency Quantization Library");
	m->declare(m->metaInterface, "quantizers.lib/version", "1.1.2");
	m->declare(m->metaInterface, "signals.lib/name", "Faust Signal Routing Library");
	m->declare(m->metaInterface, "signals.lib/version", "1.6.0");
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
	deletemydspSIG1(sig1);
}

void instanceResetUserInterfacemydsp(mydsp* dsp) {
	dsp->fVslider0 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider1 = (FAUSTFLOAT)(1.0f);
	dsp->fVslider2 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider3 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider4 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider5 = (FAUSTFLOAT)(0.1f);
	dsp->fVslider6 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider7 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider8 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider9 = (FAUSTFLOAT)(0.01f);
	dsp->fVslider10 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider11 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider12 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider13 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider14 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider15 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider16 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider17 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider18 = (FAUSTFLOAT)(8.0f);
	dsp->fVslider19 = (FAUSTFLOAT)(0.0f);
	dsp->fButton0 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider20 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider21 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider22 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider23 = (FAUSTFLOAT)(0.01f);
	dsp->fVslider24 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider25 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider26 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider27 = (FAUSTFLOAT)(0.5f);
	dsp->fCheckbox0 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider0 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider28 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider29 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider30 = (FAUSTFLOAT)(0.0f);
	dsp->fCheckbox1 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider1 = (FAUSTFLOAT)(0.5f);
	dsp->fHslider2 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider31 = (FAUSTFLOAT)(0.001f);
	dsp->fEntry0 = (FAUSTFLOAT)(4e+01f);
	dsp->fVslider32 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider33 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider3 = (FAUSTFLOAT)(1.0f);
	dsp->fVslider34 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider35 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider36 = (FAUSTFLOAT)(1.0f);
	dsp->fVslider37 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider38 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider39 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider4 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider40 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider41 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider42 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider43 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider44 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider45 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider46 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider47 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider5 = (FAUSTFLOAT)(0.0f);
	dsp->fCheckbox2 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider48 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider49 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider50 = (FAUSTFLOAT)(0.0f);
	dsp->fHslider6 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider51 = (FAUSTFLOAT)(1.0f);
	dsp->fVslider52 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider53 = (FAUSTFLOAT)(0.0f);
	dsp->fVslider54 = (FAUSTFLOAT)(1024.0f);
	dsp->fVslider55 = (FAUSTFLOAT)(0.4f);
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
			dsp->fRec9[l1] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l2;
		for (l2 = 0; l2 < 2; l2 = l2 + 1) {
			dsp->fVec1[l2] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l3;
		for (l3 = 0; l3 < 2; l3 = l3 + 1) {
			dsp->iRec10[l3] = 0;
		}
	}
	/* C99 loop */
	{
		int l4;
		for (l4 = 0; l4 < 2; l4 = l4 + 1) {
			dsp->fRec8[l4] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l5;
		for (l5 = 0; l5 < 2; l5 = l5 + 1) {
			dsp->fRec12[l5] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l6;
		for (l6 = 0; l6 < 2; l6 = l6 + 1) {
			dsp->iRec13[l6] = 0;
		}
	}
	/* C99 loop */
	{
		int l7;
		for (l7 = 0; l7 < 2; l7 = l7 + 1) {
			dsp->fRec7[l7] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l8;
		for (l8 = 0; l8 < 2; l8 = l8 + 1) {
			dsp->fRec6[l8] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l9;
		for (l9 = 0; l9 < 2; l9 = l9 + 1) {
			dsp->fRec1[l9] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l10;
		for (l10 = 0; l10 < 2; l10 = l10 + 1) {
			dsp->fRec2[l10] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l11;
		for (l11 = 0; l11 < 2; l11 = l11 + 1) {
			dsp->fVec2[l11] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l12;
		for (l12 = 0; l12 < 2; l12 = l12 + 1) {
			dsp->iRec14[l12] = 0;
		}
	}
	/* C99 loop */
	{
		int l13;
		for (l13 = 0; l13 < 2; l13 = l13 + 1) {
			dsp->fRec15[l13] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l14;
		for (l14 = 0; l14 < 2; l14 = l14 + 1) {
			dsp->iRec19[l14] = 0;
		}
	}
	/* C99 loop */
	{
		int l15;
		for (l15 = 0; l15 < 2; l15 = l15 + 1) {
			dsp->iRec17[l15] = 0;
		}
	}
	/* C99 loop */
	{
		int l16;
		for (l16 = 0; l16 < 2; l16 = l16 + 1) {
			dsp->iVec3[l16] = 0;
		}
	}
	/* C99 loop */
	{
		int l17;
		for (l17 = 0; l17 < 2; l17 = l17 + 1) {
			dsp->fRec20[l17] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l18;
		for (l18 = 0; l18 < 2; l18 = l18 + 1) {
			dsp->fRec18[l18] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l19;
		for (l19 = 0; l19 < 2; l19 = l19 + 1) {
			dsp->iRec21[l19] = 0;
		}
	}
	/* C99 loop */
	{
		int l20;
		for (l20 = 0; l20 < 2; l20 = l20 + 1) {
			dsp->fRec23[l20] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l21;
		for (l21 = 0; l21 < 2; l21 = l21 + 1) {
			dsp->fRec22[l21] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l22;
		for (l22 = 0; l22 < 2; l22 = l22 + 1) {
			dsp->fRec3[l22] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l23;
		for (l23 = 0; l23 < 2; l23 = l23 + 1) {
			dsp->fRec4[l23] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l24;
		for (l24 = 0; l24 < 2; l24 = l24 + 1) {
			dsp->fRec26[l24] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l25;
		for (l25 = 0; l25 < 2; l25 = l25 + 1) {
			dsp->fRec24[l25] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l26;
		for (l26 = 0; l26 < 2; l26 = l26 + 1) {
			dsp->fRec27[l26] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l27;
		for (l27 = 0; l27 < 2; l27 = l27 + 1) {
			dsp->fRec28[l27] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l28;
		for (l28 = 0; l28 < 2; l28 = l28 + 1) {
			dsp->fVec4[l28] = 0.0f;
		}
	}
	dsp->IOTA0 = 0;
	/* C99 loop */
	{
		int l29;
		for (l29 = 0; l29 < 4096; l29 = l29 + 1) {
			dsp->fVec5[l29] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l30;
		for (l30 = 0; l30 < 2; l30 = l30 + 1) {
			dsp->fRec29[l30] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l31;
		for (l31 = 0; l31 < 3; l31 = l31 + 1) {
			dsp->fRec30[l31] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l32;
		for (l32 = 0; l32 < 2; l32 = l32 + 1) {
			dsp->fRec5[l32] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l33;
		for (l33 = 0; l33 < 2; l33 = l33 + 1) {
			dsp->fRec40[l33] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l34;
		for (l34 = 0; l34 < 2; l34 = l34 + 1) {
			dsp->fRec41[l34] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l35;
		for (l35 = 0; l35 < 2; l35 = l35 + 1) {
			dsp->fRec43[l35] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l36;
		for (l36 = 0; l36 < 2; l36 = l36 + 1) {
			dsp->fVec6[l36] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l37;
		for (l37 = 0; l37 < 4096; l37 = l37 + 1) {
			dsp->fVec7[l37] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l38;
		for (l38 = 0; l38 < 2; l38 = l38 + 1) {
			dsp->fRec42[l38] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l41;
		for (l41 = 0; l41 < 2; l41 = l41 + 1) {
			dsp->fRec45[l41] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l42;
		for (l42 = 0; l42 < 3; l42 = l42 + 1) {
			dsp->fVec9[l42] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l43;
		for (l43 = 0; l43 < 2; l43 = l43 + 1) {
			dsp->fVec10[l43] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l44;
		for (l44 = 0; l44 < 2; l44 = l44 + 1) {
			dsp->fRec39[l44] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l45;
		for (l45 = 0; l45 < 2; l45 = l45 + 1) {
			dsp->fVec11[l45] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l46;
		for (l46 = 0; l46 < 2; l46 = l46 + 1) {
			dsp->fRec38[l46] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l47;
		for (l47 = 0; l47 < 2; l47 = l47 + 1) {
			dsp->fVec12[l47] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l48;
		for (l48 = 0; l48 < 2; l48 = l48 + 1) {
			dsp->fRec37[l48] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l49;
		for (l49 = 0; l49 < 2; l49 = l49 + 1) {
			dsp->fVec13[l49] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l50;
		for (l50 = 0; l50 < 2; l50 = l50 + 1) {
			dsp->fRec36[l50] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l51;
		for (l51 = 0; l51 < 2; l51 = l51 + 1) {
			dsp->fVec14[l51] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l52;
		for (l52 = 0; l52 < 2; l52 = l52 + 1) {
			dsp->fRec35[l52] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l53;
		for (l53 = 0; l53 < 2; l53 = l53 + 1) {
			dsp->fVec15[l53] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l54;
		for (l54 = 0; l54 < 2; l54 = l54 + 1) {
			dsp->fRec34[l54] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l55;
		for (l55 = 0; l55 < 2; l55 = l55 + 1) {
			dsp->fVec16[l55] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l56;
		for (l56 = 0; l56 < 2; l56 = l56 + 1) {
			dsp->fRec33[l56] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l57;
		for (l57 = 0; l57 < 2; l57 = l57 + 1) {
			dsp->fVec17[l57] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l58;
		for (l58 = 0; l58 < 2; l58 = l58 + 1) {
			dsp->fRec32[l58] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l59;
		for (l59 = 0; l59 < 2; l59 = l59 + 1) {
			dsp->fRec31[l59] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l60;
		for (l60 = 0; l60 < 2; l60 = l60 + 1) {
			dsp->fVec18[l60] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l61;
		for (l61 = 0; l61 < 2; l61 = l61 + 1) {
			dsp->fRec0[l61] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l62;
		for (l62 = 0; l62 < 16384; l62 = l62 + 1) {
			dsp->fVec19[l62] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l63;
		for (l63 = 0; l63 < 2; l63 = l63 + 1) {
			dsp->fRec47[l63] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l64;
		for (l64 = 0; l64 < 2; l64 = l64 + 1) {
			dsp->fRec48[l64] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l65;
		for (l65 = 0; l65 < 2; l65 = l65 + 1) {
			dsp->fRec49[l65] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l66;
		for (l66 = 0; l66 < 2; l66 = l66 + 1) {
			dsp->fRec50[l66] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l67;
		for (l67 = 0; l67 < 2; l67 = l67 + 1) {
			dsp->fRec46[l67] = 0.0f;
		}
	}
	/* C99 loop */
	{
		int l68;
		for (l68 = 0; l68 < 2; l68 = l68 + 1) {
			dsp->fRec51[l68] = 0.0f;
		}
	}
}

void instanceConstantsmydsp(mydsp* dsp, int sample_rate) {
	dsp->fSampleRate = sample_rate;
	dsp->fConst0 = fminf(1.92e+05f, fmaxf(1.0f, (float)(dsp->fSampleRate)));
	dsp->fConst1 = 1.0f / dsp->fConst0;
	mydspSIG0* sig0 = newmydspSIG0();
	instanceInitmydspSIG0(sig0, sample_rate);
	fillmydspSIG0(sig0, 32, dsp->ftbl0);
	dsp->fConst2 = 0.5f * dsp->fConst0;
	dsp->fConst3 = 0.25f * dsp->fConst0;
	dsp->fConst4 = 4.0f / dsp->fConst0;
	dsp->fConst5 = tanf(314.15927f / dsp->fConst0);
	dsp->fConst6 = 2.0f * (1.0f - 1.0f / mydsp_faustpower2_f(dsp->fConst5));
	dsp->fConst7 = 1.0f / dsp->fConst5;
	dsp->fConst8 = (dsp->fConst7 + -1.4142135f) / dsp->fConst5 + 1.0f;
	dsp->fConst9 = 1.0f / ((dsp->fConst7 + 1.4142135f) / dsp->fConst5 + 1.0f);
	dsp->fConst10 = 3.1415927f / dsp->fConst0;
	dsp->fConst11 = 44.1f / dsp->fConst0;
	dsp->fConst12 = 1.0f - dsp->fConst11;
	dsp->fConst13 = 1.0f / tanf(37699.113f / dsp->fConst0);
	dsp->fConst14 = 1.0f - dsp->fConst13;
	dsp->fConst15 = 1.0f / (dsp->fConst13 + 1.0f);
	dsp->fConst16 = 1e+01f / dsp->fConst0;
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
	ui_interface->openVerticalBox(ui_interface->uiInterface, "M0SS-201");
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
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider4, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider4, "midi", "13");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider4, "mod", "8");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "index", &dsp->fHslider4, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "alt-sel", "8");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "midi", "14");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider3, "style", "radio{'AM':0;'FM':1;'TM':2}");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "type", &dsp->fHslider3, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "principal");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider6, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider6, "alt", "17");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider6, "midi", "15");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "tune", &dsp->fHslider6, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)-2.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "midi", "16");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "sel", "17");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider5, "style", "radio{'SINE':0;'TRI':1;'BOTH':2}");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "waveform", &dsp->fHslider5, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox2, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox2, "alt-sel", "20");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fCheckbox2, "midi", "17");
	ui_interface->addCheckButton(ui_interface->uiInterface, "quantize", &dsp->fCheckbox2);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fEntry0, "hidden", "1");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fEntry0, "pitch", "");
	ui_interface->addNumEntry(ui_interface->uiInterface, "freq", &dsp->fEntry0, (FAUSTFLOAT)4e+01f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)127.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fButton0, "gate", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fButton0, "hidden", "1");
	ui_interface->addButton(ui_interface->uiInterface, "gate", &dsp->fButton0);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "mixer");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider51, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider51, "midi", "20");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider51, "mod", "18");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "principal", &dsp->fVslider51, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider45, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider45, "midi", "21");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider45, "mod", "9");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "modulator", &dsp->fVslider45, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider44, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider44, "alt", "9");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider44, "midi", "22");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "stochastic", &dsp->fVslider44, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider46, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider46, "alt", "18");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider46, "midi", "23");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "envelope", &dsp->fVslider46, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "wavefolder");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider47, "midi", "19");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider47, "mod", "20");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "bias", &dsp->fVslider47, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)-0.5f, (FAUSTFLOAT)0.5f, (FAUSTFLOAT)0.01f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider43, "midi", "18");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider43, "mod", "23");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "timbre", &dsp->fVslider43, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)4e+01f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "3", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "amplifier");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "alt-sel", "19");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "midi", "24");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider0, "style", "radio{'LPG':0;'VCA':1}");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mode", &dsp->fVslider0, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider55, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider55, "alt", "19");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider55, "midi", "25");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "volume", &dsp->fVslider55, (FAUSTFLOAT)0.4f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider31, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider31, "alt", "23");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider31, "midi", "26");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "glide", &dsp->fVslider31, (FAUSTFLOAT)0.001f, (FAUSTFLOAT)0.001f, (FAUSTFLOAT)0.99f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "4", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "delay");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider53, "alt", "25");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider53, "midi", "28");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "feedback", &dsp->fVslider53, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider52, "alt", "24");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider52, "midi", "29");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mix", &dsp->fVslider52, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.5f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider54, "alt", "16");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider54, "midi", "27");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "time", &dsp->fVslider54, (FAUSTFLOAT)1024.0f, (FAUSTFLOAT)1024.0f, (FAUSTFLOAT)8192.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "modulators");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "mod oscillator");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider1, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider1, "midi", "9");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider1, "mod", "2");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "course", &dsp->fHslider1, (FAUSTFLOAT)0.5f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider2, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider2, "alt", "2");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fHslider2, "midi", "10");
	ui_interface->addHorizontalSlider(ui_interface->uiInterface, "fine", &dsp->fHslider2, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)-0.03f, (FAUSTFLOAT)0.03f, (FAUSTFLOAT)0.0001f);
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
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph0, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph0, "MO", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "MO", &dsp->fVbargraph0, (FAUSTFLOAT)-1.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider14, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider14, "midi", "5");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider14, "sel", "11");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider14, "style", "radio{'SEPERATE':0;'SG':1;'ENV':2}");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "gate routing", &dsp->fVslider14, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "stochastic generator");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider18, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider18, "midi", "1");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider18, "mod", "1");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "length", &dsp->fVslider18, (FAUSTFLOAT)8.0f, (FAUSTFLOAT)4.0f, (FAUSTFLOAT)32.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider5, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider5, "midi", "2");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider5, "mod", "5");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "rate", &dsp->fVslider5, (FAUSTFLOAT)0.1f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider13, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider13, "midi", "3");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider13, "mod", "7");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "recall", &dsp->fVslider13, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.01f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider9, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider9, "midi", "4");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider9, "mod", "3");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "smooth", &dsp->fVslider9, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)0.25f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph1, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph1, "SG", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "SG", &dsp->fVbargraph1, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "3", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "envelope");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "midi", "6");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider23, "mod", "10");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "attack", &dsp->fVslider23, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider27, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider27, "midi", "7");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider27, "mod", "14");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "release", &dsp->fVslider27, (FAUSTFLOAT)0.5f, (FAUSTFLOAT)0.01f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "midi", "8");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "sel", "12");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider19, "style", "radio{'AR':0;'ASR':1;'LOOP':2}");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mode", &dsp->fVslider19, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)2.0f, (FAUSTFLOAT)1.0f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph2, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVbargraph2, "LE", "");
	ui_interface->addVerticalBargraph(ui_interface->uiInterface, "LE", &dsp->fVbargraph2, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openVerticalBox(ui_interface->uiInterface, "fb mod");
	ui_interface->declare(ui_interface->uiInterface, 0, "0", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "from SG");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider16, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider16, "midi", "42");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_len", &dsp->fVslider16, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider41, "10", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider41, "midi", "52");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2fold", &dsp->fVslider41, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider33, "11", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider33, "midi", "53");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2amp", &dsp->fVslider33, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider3, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider3, "midi", "43");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_rate", &dsp->fVslider3, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider11, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider11, "midi", "44");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_recall", &dsp->fVslider11, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider7, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider7, "midi", "45");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2sg_smooth", &dsp->fVslider7, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider22, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider22, "midi", "46");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2le_attack", &dsp->fVslider22, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider26, "5", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider26, "midi", "47");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2le_release", &dsp->fVslider26, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider29, "6", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider29, "midi", "48");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2mo_freq", &dsp->fVslider29, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider49, "7", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider49, "midi", "49");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2po_freq", &dsp->fVslider49, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider39, "8", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider39, "midi", "50");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2vca_ctrl", &dsp->fVslider39, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider34, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider34, "midi", "51");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "sg2vca_input", &dsp->fVslider34, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "1", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "from LE");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider17, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider17, "midi", "30");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_len", &dsp->fVslider17, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider42, "10", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider42, "midi", "40");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2fold", &dsp->fVslider42, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider1, "11", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider1, "midi", "41");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2amp", &dsp->fVslider1, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider4, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider4, "midi", "31");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_rate", &dsp->fVslider4, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider12, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider12, "midi", "32");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_recall", &dsp->fVslider12, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider8, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider8, "midi", "33");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2sg_smooth", &dsp->fVslider8, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider21, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider21, "midi", "34");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2le_attack", &dsp->fVslider21, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider25, "5", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider25, "midi", "35");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2le_release", &dsp->fVslider25, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider30, "6", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider30, "midi", "36");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2mo_freq", &dsp->fVslider30, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider50, "7", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider50, "midi", "37");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2po_freq", &dsp->fVslider50, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider38, "8", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider38, "midi", "38");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2vca_ctrl", &dsp->fVslider38, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider35, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider35, "midi", "39");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "le2vca_input", &dsp->fVslider35, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->declare(ui_interface->uiInterface, 0, "2", "");
	ui_interface->openHorizontalBox(ui_interface->uiInterface, "from MO");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider15, "0", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider15, "midi", "54");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_len", &dsp->fVslider15, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider40, "10", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider40, "midi", "64");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2fold", &dsp->fVslider40, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider32, "11", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider32, "midi", "65");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2amp", &dsp->fVslider32, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider2, "1", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider2, "midi", "55");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_rate", &dsp->fVslider2, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider10, "2", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider10, "midi", "56");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_recall", &dsp->fVslider10, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider6, "3", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider6, "midi", "57");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2sg_smooth", &dsp->fVslider6, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider20, "4", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider20, "midi", "58");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2le_attack", &dsp->fVslider20, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider24, "5", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider24, "midi", "59");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2le_release", &dsp->fVslider24, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider28, "6", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider28, "midi", "60");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2mo_freq", &dsp->fVslider28, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider48, "7", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider48, "midi", "61");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2po_freq", &dsp->fVslider48, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider37, "8", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider37, "midi", "62");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2vca_ctrl", &dsp->fVslider37, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider36, "9", "");
	ui_interface->declare(ui_interface->uiInterface, &dsp->fVslider36, "midi", "63");
	ui_interface->addVerticalSlider(ui_interface->uiInterface, "mo2vca_input", &dsp->fVslider36, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.0f, (FAUSTFLOAT)1.0f, (FAUSTFLOAT)0.001f);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
	ui_interface->closeBox(ui_interface->uiInterface);
}

void computemydsp(mydsp* dsp, int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
	FAUSTFLOAT* output0 = outputs[0];
	FAUSTFLOAT* output1 = outputs[1];
	int iSlow0 = (int)((float)(dsp->fVslider0));
	float fSlow1 = (float)(dsp->fVslider1);
	int iSlow2 = fSlow1 == 0.0f;
	float fSlow3 = (float)(dsp->fVslider2);
	float fSlow4 = (float)(dsp->fVslider3);
	float fSlow5 = (float)(dsp->fVslider4);
	float fSlow6 = 199.99f * mydsp_faustpower2_f((float)(dsp->fVslider5));
	float fSlow7 = (float)(dsp->fVslider6);
	float fSlow8 = (float)(dsp->fVslider7);
	float fSlow9 = (float)(dsp->fVslider8);
	float fSlow10 = (float)(dsp->fVslider9);
	float fSlow11 = (float)(dsp->fVslider10);
	float fSlow12 = (float)(dsp->fVslider11);
	float fSlow13 = (float)(dsp->fVslider12);
	float fSlow14 = (float)(dsp->fVslider13);
	float fSlow15 = (float)(dsp->fVslider14);
	int iSlow16 = fSlow15 == 2.0f;
	float fSlow17 = (float)(dsp->fVslider15);
	float fSlow18 = (float)(dsp->fVslider16);
	float fSlow19 = (float)(dsp->fVslider17);
	int iSlow20 = (int)((float)(dsp->fVslider18));
	float fSlow21 = (float)(dsp->fVslider19);
	int iSlow22 = fSlow21 == 0.0f;
	int iSlow23 = fSlow21 == 1.0f;
	int iSlow24 = fSlow15 == 1.0f;
	float fSlow25 = (float)(dsp->fButton0);
	float fSlow26 = (float)(dsp->fVslider20);
	float fSlow27 = (float)(dsp->fVslider21);
	float fSlow28 = (float)(dsp->fVslider22);
	float fSlow29 = (float)(dsp->fVslider23);
	float fSlow30 = (float)(dsp->fVslider24);
	float fSlow31 = (float)(dsp->fVslider25);
	float fSlow32 = (float)(dsp->fVslider26);
	float fSlow33 = (float)(dsp->fVslider27);
	int iSlow34 = fSlow21 == 2.0f;
	float fSlow35 = (float)(dsp->fCheckbox0);
	int iSlow36 = fSlow35 == 1.0f;
	float fSlow37 = (float)(dsp->fHslider0);
	int iSlow38 = fSlow37 == 0.0f;
	int iSlow39 = fSlow37 == 1.0f;
	float fSlow40 = (float)(dsp->fVslider28);
	float fSlow41 = (float)(dsp->fVslider29);
	float fSlow42 = (float)(dsp->fVslider30);
	int iSlow43 = (int)(fSlow35);
	int iSlow44 = (int)((float)(dsp->fCheckbox1));
	float fSlow45 = fmaxf(0.0f, fminf(1.0f, (float)(dsp->fHslider2) + (float)(dsp->fHslider1)));
	float fSlow46 = 2.98e+03f * mydsp_faustpower2_f(fSlow45) + 2e+01f;
	float fSlow47 = (float)(dsp->fVslider31);
	int iSlow48 = fabsf(fSlow47) < 1.1920929e-07f;
	float fSlow49 = ((iSlow48) ? 0.0f : expf(-(dsp->fConst1 / ((iSlow48) ? 1.0f : fSlow47))));
	float fSlow50 = 4.4e+02f * powf(2.0f, 0.083333336f * ((float)(dsp->fEntry0) + -69.0f)) * (1.0f - fSlow49);
	float fSlow51 = powf(2.0f, 4.0f * fSlow45 + -2.0f);
	float fSlow52 = (float)(dsp->fVslider32);
	float fSlow53 = 0.5f * (float)(dsp->fVslider33);
	float fSlow54 = (float)(dsp->fHslider3);
	int iSlow55 = fSlow54 == 2.0f;
	float fSlow56 = (float)(dsp->fVslider34);
	float fSlow57 = (float)(dsp->fVslider35);
	float fSlow58 = (float)(dsp->fVslider36);
	float fSlow59 = (float)(dsp->fVslider37);
	float fSlow60 = (float)(dsp->fVslider38);
	float fSlow61 = (float)(dsp->fVslider39);
	float fSlow62 = (float)(dsp->fHslider4);
	float fSlow63 = (float)(dsp->fVslider40);
	float fSlow64 = 0.5f * (float)(dsp->fVslider41);
	float fSlow65 = (float)(dsp->fVslider42);
	float fSlow66 = dsp->fConst11 * (float)(dsp->fVslider43);
	float fSlow67 = (float)(dsp->fVslider44);
	float fSlow68 = (float)(dsp->fVslider45);
	float fSlow69 = (float)(dsp->fVslider46);
	float fSlow70 = dsp->fConst11 * (float)(dsp->fVslider47);
	int iSlow71 = fSlow54 == 0.0f;
	float fSlow72 = (float)(dsp->fHslider5);
	int iSlow73 = fSlow72 == 0.0f;
	int iSlow74 = fSlow72 == 1.0f;
	int iSlow75 = (int)((float)(dsp->fCheckbox2));
	float fSlow76 = 2.0f * (float)(dsp->fVslider48);
	float fSlow77 = 2.0f * (float)(dsp->fVslider49);
	float fSlow78 = 2.0f * (float)(dsp->fVslider50);
	int iSlow79 = fSlow54 == 1.0f;
	float fSlow80 = powf(2.0f, (float)(dsp->fHslider6));
	float fSlow81 = 0.0022727272f * fSlow80;
	float fSlow82 = (float)(dsp->fVslider51);
	float fSlow83 = (float)(dsp->fVslider52);
	float fSlow84 = 1.0f - fSlow83;
	float fSlow85 = (float)(dsp->fVslider53);
	float fSlow86 = (float)(dsp->fVslider54);
	float fSlow87 = dsp->fConst11 * (float)(dsp->fVslider55);
	/* C99 loop */
	{
		int i0;
		for (i0 = 0; i0 < count; i0 = i0 + 1) {
			dsp->iVec0[0] = 1;
			float fTemp0 = fSlow6 + 199.9f * mydsp_faustpower2_f(fSlow5 * dsp->fRec3[1] + fSlow4 * dsp->fRec1[1] + fSlow3 * dsp->fRec5[1]) + 0.11f;
			float fTemp1 = fmaxf(0.0f, fminf(1.0f, fSlow10 + fSlow9 * dsp->fRec3[1] + fSlow8 * dsp->fRec1[1] + fSlow7 * dsp->fRec5[1])) / fTemp0;
			int iTemp2 = fabsf(fTemp1) < 1.1920929e-07f;
			float fTemp3 = ((iTemp2) ? 0.0f : expf(-(dsp->fConst1 / ((iTemp2) ? 1.0f : fTemp1))));
			int iTemp4 = 1 - dsp->iVec0[1];
			float fTemp5 = ((iTemp4) ? 0.0f : dsp->fRec9[1] + dsp->fConst1 * fTemp0);
			dsp->fRec9[0] = fTemp5 - floorf(fTemp5);
			float fTemp6 = ((iSlow16) ? ((iSlow16) ? dsp->fRec4[1] : 0.0f) : (float)(dsp->fRec9[0] <= 0.5f));
			dsp->fVec1[0] = fTemp6;
			int iTemp7 = (dsp->fVec1[1] <= 0.0f) & (fTemp6 > 0.0f);
			float fTemp8 = (float)(iTemp7);
			int iTemp9 = 1103515245 * (dsp->iRec10[1] + 12345);
			dsp->iRec10[0] = 1103515245 * (iTemp9 + 12345);
			int iRec11 = iTemp9;
			float fTemp10 = (float)(1 - iTemp7);
			dsp->fRec8[0] = dsp->fRec8[1] * fTemp10 + 4.656613e-10f * (float)(iRec11) * fTemp8;
			dsp->fRec12[0] = fTemp10 * dsp->fRec12[1] + 4.656613e-10f * (float)(dsp->iRec10[0]) * fTemp8;
			int iTemp11 = max_i(1, min_i(32, iSlow20 + (int)(32.0f * (fSlow19 * dsp->fRec3[1] + fSlow18 * dsp->fRec1[1] + fSlow17 * dsp->fRec5[1])))) + 1;
			dsp->iRec13[0] = dsp->iRec13[1] + (fTemp6 > dsp->fVec1[1]);
			int iTemp12 = dsp->iRec13[0] % iTemp11;
			dsp->ftbl0[max_i(0, min_i(iTemp12, 31))] = (((0.5f * (dsp->fRec8[0] + 1.0f)) > fmaxf(0.0f, fminf(1.0f, fSlow14 + fSlow13 * dsp->fRec3[1] + fSlow12 * dsp->fRec1[1] + fSlow11 * dsp->fRec5[1]))) ? 0.5f * (dsp->fRec12[0] + 1.0f) : dsp->fRec7[1]);
			dsp->fRec7[0] = dsp->ftbl0[max_i(0, min_i((iTemp12 + 1) % iTemp11, 31))];
			dsp->fRec6[0] = dsp->fRec7[0] * (1.0f - fTemp3) + fTemp3 * dsp->fRec6[1];
			dsp->fRec1[0] = dsp->fRec6[0];
			dsp->fRec2[0] = fTemp6;
			float fTemp13 = ((iSlow24) ? dsp->fRec2[1] : fSlow25);
			dsp->fVec2[0] = fTemp13;
			float fTemp14 = fTemp13 - dsp->fVec2[1];
			int iTemp15 = (int)(fTemp14 * (float)(fTemp14 > 0.0f));
			int iTemp16 = ((iTemp15) ? 0 : ((dsp->iRec14[1]) ? ((dsp->fRec15[1] > 0.0f) ? 1 : 0) : ((dsp->fRec15[1] < 1.0f) ? 0 : 1)));
			dsp->iRec14[0] = iTemp16;
			float fTemp17 = fmaxf(0.03f, fminf(0.97f, ((dsp->iRec14[1]) ? 0.3f : 0.7f)));
			float fTemp18 = fTemp17 + -1.0f;
			float fTemp19 = dsp->fRec15[1] * fTemp18 / (fTemp17 * (2.0f * dsp->fRec15[1] + -1.0f) - dsp->fRec15[1]);
			float fTemp20 = fSlow29 + fSlow28 * dsp->fRec1[1] + fSlow27 * dsp->fRec3[1] + fSlow26 * dsp->fRec5[1];
			float fTemp21 = 1.0f / fmaxf(1.0f, dsp->fConst0 * ((fTemp20 < 0.01f) ? 0.01f : fTemp20));
			float fTemp22 = fSlow33 + fSlow32 * dsp->fRec1[1] + fSlow31 * dsp->fRec3[1] + fSlow30 * dsp->fRec5[1];
			float fTemp23 = fmaxf(1.0f, dsp->fConst0 * ((fTemp22 < 0.01f) ? 0.01f : fTemp22));
			float fTemp24 = 1.0f / fTemp23;
			dsp->fRec15[0] = ((iTemp15) ? 0.0f : fmaxf(0.0f, fminf(1.0f, dsp->fRec15[1] - ((dsp->iRec14[1]) ? -fTemp24 : fTemp21) * fTemp17 * fTemp18 / mydsp_faustpower2_f(fTemp17 * (2.0f * fTemp19 + -1.0f) + (1.0f - fTemp19)))));
			int iRec16 = ((((dsp->iRec14[1] == (1 & iTemp16)) == 0) | iTemp15) ? 1 : 0);
			int iTemp25 = fTemp13 > 0.0f;
			int iTemp26 = dsp->iRec17[1] == 0;
			int iTemp27 = dsp->iRec17[1] == 1;
			dsp->iRec19[0] = dsp->iRec19[1] + 1;
			int iTemp28 = dsp->iRec19[1] == 0;
			float fTemp29 = ((iTemp28) ? 0.0f : dsp->fRec18[1]);
			dsp->iRec17[0] = ((iTemp25) ? ((iTemp26) ? 1 : ((iTemp27) ? ((fTemp29 < 1.0f) ? 1 : 2) : 2)) : 0);
			float fTemp30 = (float)(((dsp->iRec17[1] == 2) ? 1 : 0));
			int iTemp31 = fTemp13 == 0.0f;
			dsp->iVec3[0] = iTemp31;
			int iTemp32 = (dsp->iVec3[1] <= 0) & (iTemp31 > 0);
			float fTemp33 = (float)(iTemp32);
			float fTemp34 = (float)(1 - iTemp32);
			dsp->fRec20[0] = dsp->fRec20[1] * fTemp34 + ((iTemp28) ? 1.0f : fTemp29) * fTemp33;
			float fTemp35 = ((iTemp26) ? dsp->fRec20[0] : 1.0f);
			float fTemp36 = ((fTemp30 == fTemp35) ? 0.5f : (fTemp29 - fTemp30) / (fTemp35 - fTemp30));
			float fTemp37 = fmaxf(0.03f, fminf(0.97f, ((iTemp26) ? 0.3f : ((iTemp27) ? 0.7f : 0.3f))));
			float fTemp38 = fTemp37 + -1.0f;
			float fTemp39 = fTemp38 * fTemp36 / (fTemp37 * (2.0f * fTemp36 + -1.0f) - fTemp36);
			dsp->fRec18[0] = fmaxf(fTemp30, fminf(1.0f, fTemp29 - ((iTemp26) ? -(dsp->fRec20[0] / fTemp23) : ((iTemp27) ? fTemp21 : 0.0f)) * fTemp37 * fTemp38 / mydsp_faustpower2_f(fTemp37 * (2.0f * fTemp39 + -1.0f) + (1.0f - fTemp39))));
			int iTemp40 = dsp->iRec21[1] == 0;
			int iTemp41 = dsp->iRec21[1] == 1;
			float fTemp42 = ((iTemp28) ? 0.0f : dsp->fRec22[1]);
			dsp->iRec21[0] = ((iTemp25) ? ((iTemp40) ? 1 : ((iTemp41) ? ((fTemp42 < 1.0f) ? 1 : 2) : 2)) : 0);
			dsp->fRec23[0] = fTemp34 * dsp->fRec23[1] + fTemp33 * ((iTemp28) ? 1.0f : fTemp42);
			float fTemp43 = ((iTemp40) ? dsp->fRec23[0] : 1.0f);
			float fTemp44 = ((0.0f == fTemp43) ? 0.5f : fTemp42 / fTemp43);
			float fTemp45 = fmaxf(0.03f, fminf(0.97f, ((iTemp40) ? 0.3f : ((iTemp41) ? 0.7f : 0.3f))));
			float fTemp46 = fTemp45 + -1.0f;
			float fTemp47 = fTemp46 * fTemp44 / (fTemp45 * (2.0f * fTemp44 + -1.0f) - fTemp44);
			dsp->fRec22[0] = fmaxf(0.0f, fminf(1.0f, fTemp42 - ((iTemp40) ? -(dsp->fRec23[0] / fTemp23) : ((iTemp41) ? fTemp21 : -fTemp24)) * fTemp45 * fTemp46 / mydsp_faustpower2_f(fTemp45 * (2.0f * fTemp47 + -1.0f) + (1.0f - fTemp47))));
			dsp->fRec3[0] = ((iSlow22) ? dsp->fRec22[0] : ((iSlow23) ? dsp->fRec18[0] : dsp->fRec15[0]));
			dsp->fRec4[0] = ((iSlow34) ? (float)(iRec16) : fTemp13);
			float fTemp48 = powf(2.0f, 2.0f * (fSlow42 * dsp->fRec3[1] + fSlow41 * dsp->fRec1[1] + fSlow40 * dsp->fRec5[1]));
			dsp->fRec26[0] = fSlow50 + fSlow49 * dsp->fRec26[1];
			float fTemp49 = ((iSlow44) ? fSlow51 * dsp->fRec26[0] : fSlow46);
			float fTemp50 = ((iSlow43) ? 0.1f * fTemp49 : fTemp49);
			float fTemp51 = fTemp50 * fTemp48;
			float fTemp52 = fmaxf(1.1920929e-07f, fabsf(fTemp51));
			float fTemp53 = dsp->fRec24[1] + dsp->fConst1 * fTemp52;
			float fTemp54 = fTemp53 + -1.0f;
			int iTemp55 = fTemp54 < 0.0f;
			dsp->fRec24[0] = ((iTemp55) ? fTemp53 : fTemp54);
			float fRec25 = ((iTemp55) ? fTemp53 : fTemp53 + fTemp54 * (1.0f - dsp->fConst0 / fTemp52));
			float fTemp56 = ((iTemp4) ? 0.0f : dsp->fRec27[1] + dsp->fConst1 * fTemp51);
			dsp->fRec27[0] = fTemp56 - floorf(fTemp56);
			float fTemp57 = 2.0f * dsp->fRec27[0] + -1.0f;
			float fTemp58 = fmaxf(fTemp51, 23.44895f);
			float fTemp59 = fmaxf(2e+01f, fabsf(fTemp58));
			float fTemp60 = ((iTemp4) ? 0.0f : dsp->fRec28[1] + dsp->fConst1 * fTemp59);
			dsp->fRec28[0] = fTemp60 - floorf(fTemp60);
			float fTemp61 = mydsp_faustpower2_f(2.0f * dsp->fRec28[0] + -1.0f);
			dsp->fVec4[0] = fTemp61;
			float fTemp62 = (float)(dsp->iVec0[1]);
			float fTemp63 = fTemp62 * (fTemp61 - dsp->fVec4[1]) / fTemp59;
			dsp->fVec5[dsp->IOTA0 & 4095] = fTemp63;
			float fTemp64 = fmaxf(0.0f, fminf(2047.0f, dsp->fConst2 / fTemp58));
			int iTemp65 = (int)(fTemp64);
			float fTemp66 = floorf(fTemp64);
			float fTemp67 = dsp->fConst3 * (fTemp63 - dsp->fVec5[(dsp->IOTA0 - iTemp65) & 4095] * (fTemp66 + (1.0f - fTemp64)) - (fTemp64 - fTemp66) * dsp->fVec5[(dsp->IOTA0 - (iTemp65 + 1)) & 4095]);
			dsp->fRec29[0] = fTemp67 + 0.999f * dsp->fRec29[1];
			float fTemp68 = ((iSlow38) ? ((iSlow36) ? 2.0f * (1.0f - fabsf(fTemp57)) + -1.0f : dsp->fConst4 * dsp->fRec29[0] * fTemp50 * fTemp48) : ((iSlow39) ? ((iSlow36) ? 2.0f * (float)(dsp->fRec27[0] <= 0.5f) + -1.0f : fTemp67) : ((iSlow36) ? fTemp57 : 2.0f * fRec25 + -1.0f)));
			dsp->fRec30[0] = fTemp68 - dsp->fConst9 * (dsp->fConst8 * dsp->fRec30[2] + dsp->fConst6 * dsp->fRec30[1]);
			dsp->fRec5[0] = ((iSlow36) ? dsp->fConst9 * (dsp->fRec30[2] + dsp->fRec30[0] + 2.0f * dsp->fRec30[1]) : fTemp68);
			dsp->fVbargraph0 = (FAUSTFLOAT)(dsp->fRec5[0]);
			float fTemp69 = dsp->fRec5[0];
			dsp->fVbargraph1 = (FAUSTFLOAT)(dsp->fRec1[0]);
			float fTemp70 = dsp->fRec1[0];
			float fTemp71 = fTemp70 + 1.0f;
			dsp->fVbargraph2 = (FAUSTFLOAT)(dsp->fRec3[0]);
			float fTemp72 = dsp->fRec3[0];
			float fTemp73 = ((iSlow2) ? 1.0f : fSlow1 * fTemp72 + fSlow53 * fTemp71 + fSlow52 * fTemp69);
			float fTemp74 = 1.0f / tanf(dsp->fConst10 * (mydsp_faustpower2_f(9e+01f * fTemp73) + 0.01f));
			float fTemp75 = fSlow58 * fTemp69 + fSlow57 * fTemp72 + fSlow56 * fTemp70;
			float fTemp76 = fmaxf(0.0f, fminf(1.0f, fSlow62 + 0.5f * (fSlow61 * fTemp70 + fSlow60 * fTemp72 + fSlow59 * fTemp69)));
			dsp->fRec40[0] = fSlow66 + dsp->fConst12 * dsp->fRec40[1];
			dsp->fRec41[0] = fSlow70 + dsp->fConst12 * dsp->fRec41[1];
			float fTemp77 = dsp->fRec26[0] * ((iSlow79) ? 0.75f * fTemp76 * fTemp75 + 1.0f : 1.0f) * powf(2.0f, fSlow78 * fTemp72) * powf(2.0f, fSlow77 * fTemp70) * powf(2.0f, fSlow76 * fTemp69);
			float fTemp78 = powf(2.0f, floorf(1.442695f * logf(fSlow81 * fTemp77)));
			float fTemp79 = fSlow81 * (fTemp77 / fTemp78);
			float fTemp80 = ((iSlow75) ? 4.4e+02f * fTemp78 * ((fTemp79 < 1.7817974f) ? ((fTemp79 < 1.4983071f) ? ((fTemp79 < 1.3348398f) ? ((fTemp79 < 1.1892071f) ? 1.0f : 1.1892071f) : 1.3348398f) : 1.4983071f) : 1.7817974f) : fSlow80 * fTemp77);
			float fTemp81 = fmaxf(fTemp80, 23.44895f);
			float fTemp82 = fmaxf(2e+01f, fabsf(fTemp81));
			float fTemp83 = ((iTemp4) ? 0.0f : dsp->fRec43[1] + dsp->fConst1 * fTemp82);
			dsp->fRec43[0] = fTemp83 - floorf(fTemp83);
			float fTemp84 = mydsp_faustpower2_f(2.0f * dsp->fRec43[0] + -1.0f);
			dsp->fVec6[0] = fTemp84;
			float fTemp85 = fTemp62 * (fTemp84 - dsp->fVec6[1]) / fTemp82;
			dsp->fVec7[dsp->IOTA0 & 4095] = fTemp85;
			float fTemp86 = fmaxf(0.0f, fminf(2047.0f, dsp->fConst2 / fTemp81));
			int iTemp87 = (int)(fTemp86);
			float fTemp88 = floorf(fTemp86);
			dsp->fRec42[0] = 0.999f * dsp->fRec42[1] + dsp->fConst3 * (fTemp85 - dsp->fVec7[(dsp->IOTA0 - iTemp87) & 4095] * (fTemp88 + (1.0f - fTemp86)) - (fTemp86 - fTemp88) * dsp->fVec7[(dsp->IOTA0 - (iTemp87 + 1)) & 4095]);
			float fTemp89 = dsp->fConst4 * dsp->fRec42[0] * fTemp80;
			float fTemp90 = ((iTemp4) ? 0.0f : dsp->fRec45[1] + dsp->fConst1 * fTemp80);
			dsp->fRec45[0] = fTemp90 - floorf(fTemp90);
			float fTemp91 = ftbl1mydspSIG1[max_i(0, min_i((int)(65536.0f * dsp->fRec45[0]), 65535))];
			dsp->fVec9[0] = ((iSlow73) ? fTemp91 : ((iSlow74) ? fTemp89 : 0.5f * (fTemp91 + fTemp89))) * ((iSlow71) ? 1.0f - fTemp76 * (1.0f - fTemp75) : 1.0f);
			float fTemp92 = 0.4f * (fSlow82 * dsp->fVec9[2] + dsp->fRec41[0] + fSlow69 * fTemp72 + fSlow68 * fTemp69 + fSlow67 * fTemp70) * (fmaxf(0.0f, fminf(4e+01f, dsp->fRec40[0] + 2e+01f * (fSlow65 * fTemp72 + fSlow64 * fTemp71 + fSlow63 * fTemp69 + ((iSlow55) ? fTemp76 * (0.5f * fTemp75 + 1.0f) : 0.0f)))) + 1.0f);
			float fTemp93 = ((fTemp92 < -1.6f) ? -2.0f - fTemp92 : ((fTemp92 < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(fTemp92 + 1.0f) + -0.7f : ((fTemp92 < 0.4f) ? fTemp92 : ((fTemp92 < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(fTemp92 + -1.0f) : 2.0f - fTemp92))));
			dsp->fVec10[0] = fTemp93;
			dsp->fRec39[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec39[1] - (fTemp93 + dsp->fVec10[1])));
			float fTemp94 = ((dsp->fRec39[0] < -1.6f) ? -2.0f - dsp->fRec39[0] : ((dsp->fRec39[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec39[0] + 1.0f) + -0.7f : ((dsp->fRec39[0] < 0.4f) ? dsp->fRec39[0] : ((dsp->fRec39[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec39[0] + -1.0f) : 2.0f - dsp->fRec39[0]))));
			dsp->fVec11[0] = fTemp94;
			dsp->fRec38[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec38[1] - (fTemp94 + dsp->fVec11[1])));
			float fTemp95 = ((dsp->fRec38[0] < -1.6f) ? -2.0f - dsp->fRec38[0] : ((dsp->fRec38[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec38[0] + 1.0f) + -0.7f : ((dsp->fRec38[0] < 0.4f) ? dsp->fRec38[0] : ((dsp->fRec38[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec38[0] + -1.0f) : 2.0f - dsp->fRec38[0]))));
			dsp->fVec12[0] = fTemp95;
			dsp->fRec37[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec37[1] - (fTemp95 + dsp->fVec12[1])));
			float fTemp96 = ((dsp->fRec37[0] < -1.6f) ? -2.0f - dsp->fRec37[0] : ((dsp->fRec37[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec37[0] + 1.0f) + -0.7f : ((dsp->fRec37[0] < 0.4f) ? dsp->fRec37[0] : ((dsp->fRec37[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec37[0] + -1.0f) : 2.0f - dsp->fRec37[0]))));
			dsp->fVec13[0] = fTemp96;
			dsp->fRec36[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec36[1] - (fTemp96 + dsp->fVec13[1])));
			float fTemp97 = ((dsp->fRec36[0] < -1.6f) ? -2.0f - dsp->fRec36[0] : ((dsp->fRec36[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec36[0] + 1.0f) + -0.7f : ((dsp->fRec36[0] < 0.4f) ? dsp->fRec36[0] : ((dsp->fRec36[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec36[0] + -1.0f) : 2.0f - dsp->fRec36[0]))));
			dsp->fVec14[0] = fTemp97;
			dsp->fRec35[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec35[1] - (fTemp97 + dsp->fVec14[1])));
			float fTemp98 = ((dsp->fRec35[0] < -1.6f) ? -2.0f - dsp->fRec35[0] : ((dsp->fRec35[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec35[0] + 1.0f) + -0.7f : ((dsp->fRec35[0] < 0.4f) ? dsp->fRec35[0] : ((dsp->fRec35[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec35[0] + -1.0f) : 2.0f - dsp->fRec35[0]))));
			dsp->fVec15[0] = fTemp98;
			dsp->fRec34[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec34[1] - (fTemp98 + dsp->fVec15[1])));
			float fTemp99 = ((dsp->fRec34[0] < -1.6f) ? -2.0f - dsp->fRec34[0] : ((dsp->fRec34[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec34[0] + 1.0f) + -0.7f : ((dsp->fRec34[0] < 0.4f) ? dsp->fRec34[0] : ((dsp->fRec34[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec34[0] + -1.0f) : 2.0f - dsp->fRec34[0]))));
			dsp->fVec16[0] = fTemp99;
			dsp->fRec33[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec33[1] - (fTemp99 + dsp->fVec16[1])));
			float fTemp100 = ((dsp->fRec33[0] < -1.6f) ? -2.0f - dsp->fRec33[0] : ((dsp->fRec33[0] < -0.4f) ? 0.8333333f * mydsp_faustpower2_f(dsp->fRec33[0] + 1.0f) + -0.7f : ((dsp->fRec33[0] < 0.4f) ? dsp->fRec33[0] : ((dsp->fRec33[0] < 1.6f) ? 0.7f - 0.8333333f * mydsp_faustpower2_f(dsp->fRec33[0] + -1.0f) : 2.0f - dsp->fRec33[0]))));
			dsp->fVec17[0] = fTemp100;
			dsp->fRec32[0] = -(dsp->fConst15 * (dsp->fConst14 * dsp->fRec32[1] - (fTemp100 + dsp->fVec17[1])));
			dsp->fRec31[0] = (2.5f * (dsp->fRec32[0] + dsp->fRec32[1]) - dsp->fRec31[1] * (1.0f - fTemp74)) / (fTemp74 + 1.0f);
			float fTemp101 = ((iSlow0) ? 2.5f * dsp->fRec32[0] * fTemp73 : dsp->fRec31[0] * fTemp73);
			dsp->fVbargraph3 = (FAUSTFLOAT)(fTemp101);
			float fTemp102 = fTemp101;
			dsp->fVec18[0] = fTemp102;
			dsp->fRec0[0] = 0.995f * dsp->fRec0[1] + fTemp102 - dsp->fVec18[1];
			float fTemp103 = dsp->fRec0[0] + fSlow85 * dsp->fRec46[1];
			dsp->fVec19[dsp->IOTA0 & 16383] = fTemp103;
			float fTemp104 = ((dsp->fRec47[1] != 0.0f) ? (((dsp->fRec48[1] > 0.0f) & (dsp->fRec48[1] < 1.0f)) ? dsp->fRec47[1] : 0.0f) : (((dsp->fRec48[1] == 0.0f) & (fSlow86 != dsp->fRec49[1])) ? dsp->fConst16 : (((dsp->fRec48[1] == 1.0f) & (fSlow86 != dsp->fRec50[1])) ? -dsp->fConst16 : 0.0f)));
			dsp->fRec47[0] = fTemp104;
			dsp->fRec48[0] = fmaxf(0.0f, fminf(1.0f, dsp->fRec48[1] + fTemp104));
			dsp->fRec49[0] = (((dsp->fRec48[1] >= 1.0f) & (dsp->fRec50[1] != fSlow86)) ? fSlow86 : dsp->fRec49[1]);
			dsp->fRec50[0] = (((dsp->fRec48[1] <= 0.0f) & (dsp->fRec49[1] != fSlow86)) ? fSlow86 : dsp->fRec50[1]);
			float fTemp105 = dsp->fVec19[(dsp->IOTA0 - (int)(fminf(8192.0f, fmaxf(0.0f, dsp->fRec49[0])))) & 16383];
			dsp->fRec46[0] = fTemp105 + dsp->fRec48[0] * (dsp->fVec19[(dsp->IOTA0 - (int)(fminf(8192.0f, fmaxf(0.0f, dsp->fRec50[0])))) & 16383] - fTemp105);
			dsp->fRec51[0] = fSlow87 + dsp->fConst12 * dsp->fRec51[1];
			float fTemp106 = dsp->fRec51[0] * (fSlow83 * dsp->fRec46[0] + fSlow84 * dsp->fRec0[0]);
			output0[i0] = (FAUSTFLOAT)(fTemp106);
			output1[i0] = (FAUSTFLOAT)(fTemp106);
			dsp->iVec0[1] = dsp->iVec0[0];
			dsp->fRec9[1] = dsp->fRec9[0];
			dsp->fVec1[1] = dsp->fVec1[0];
			dsp->iRec10[1] = dsp->iRec10[0];
			dsp->fRec8[1] = dsp->fRec8[0];
			dsp->fRec12[1] = dsp->fRec12[0];
			dsp->iRec13[1] = dsp->iRec13[0];
			dsp->fRec7[1] = dsp->fRec7[0];
			dsp->fRec6[1] = dsp->fRec6[0];
			dsp->fRec1[1] = dsp->fRec1[0];
			dsp->fRec2[1] = dsp->fRec2[0];
			dsp->fVec2[1] = dsp->fVec2[0];
			dsp->iRec14[1] = dsp->iRec14[0];
			dsp->fRec15[1] = dsp->fRec15[0];
			dsp->iRec19[1] = dsp->iRec19[0];
			dsp->iRec17[1] = dsp->iRec17[0];
			dsp->iVec3[1] = dsp->iVec3[0];
			dsp->fRec20[1] = dsp->fRec20[0];
			dsp->fRec18[1] = dsp->fRec18[0];
			dsp->iRec21[1] = dsp->iRec21[0];
			dsp->fRec23[1] = dsp->fRec23[0];
			dsp->fRec22[1] = dsp->fRec22[0];
			dsp->fRec3[1] = dsp->fRec3[0];
			dsp->fRec4[1] = dsp->fRec4[0];
			dsp->fRec26[1] = dsp->fRec26[0];
			dsp->fRec24[1] = dsp->fRec24[0];
			dsp->fRec27[1] = dsp->fRec27[0];
			dsp->fRec28[1] = dsp->fRec28[0];
			dsp->fVec4[1] = dsp->fVec4[0];
			dsp->IOTA0 = dsp->IOTA0 + 1;
			dsp->fRec29[1] = dsp->fRec29[0];
			dsp->fRec30[2] = dsp->fRec30[1];
			dsp->fRec30[1] = dsp->fRec30[0];
			dsp->fRec5[1] = dsp->fRec5[0];
			dsp->fRec40[1] = dsp->fRec40[0];
			dsp->fRec41[1] = dsp->fRec41[0];
			dsp->fRec43[1] = dsp->fRec43[0];
			dsp->fVec6[1] = dsp->fVec6[0];
			dsp->fRec42[1] = dsp->fRec42[0];
			dsp->fRec45[1] = dsp->fRec45[0];
			dsp->fVec9[2] = dsp->fVec9[1];
			dsp->fVec9[1] = dsp->fVec9[0];
			dsp->fVec10[1] = dsp->fVec10[0];
			dsp->fRec39[1] = dsp->fRec39[0];
			dsp->fVec11[1] = dsp->fVec11[0];
			dsp->fRec38[1] = dsp->fRec38[0];
			dsp->fVec12[1] = dsp->fVec12[0];
			dsp->fRec37[1] = dsp->fRec37[0];
			dsp->fVec13[1] = dsp->fVec13[0];
			dsp->fRec36[1] = dsp->fRec36[0];
			dsp->fVec14[1] = dsp->fVec14[0];
			dsp->fRec35[1] = dsp->fRec35[0];
			dsp->fVec15[1] = dsp->fVec15[0];
			dsp->fRec34[1] = dsp->fRec34[0];
			dsp->fVec16[1] = dsp->fVec16[0];
			dsp->fRec33[1] = dsp->fRec33[0];
			dsp->fVec17[1] = dsp->fVec17[0];
			dsp->fRec32[1] = dsp->fRec32[0];
			dsp->fRec31[1] = dsp->fRec31[0];
			dsp->fVec18[1] = dsp->fVec18[0];
			dsp->fRec0[1] = dsp->fRec0[0];
			dsp->fRec47[1] = dsp->fRec47[0];
			dsp->fRec48[1] = dsp->fRec48[0];
			dsp->fRec49[1] = dsp->fRec49[0];
			dsp->fRec50[1] = dsp->fRec50[0];
			dsp->fRec46[1] = dsp->fRec46[0];
			dsp->fRec51[1] = dsp->fRec51[0];
		}
	}
}

#ifdef __cplusplus
}
#endif

#endif
