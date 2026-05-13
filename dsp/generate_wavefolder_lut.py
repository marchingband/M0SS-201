import numpy
import matplotlib.pyplot as plt

TABLE_SIZE = 512

def main():    
    # we add four to have extras for the hermite interpolation
    x = numpy.arange(0, TABLE_SIZE + 4) / (TABLE_SIZE / 2.0) - 1.0
    x[-1] = x[-2]
    sine = numpy.sin(8 * numpy.pi * x)
    window = numpy.exp(-x * x * 4) ** 2
    bipolar_fold = sine * window + numpy.arctan(3 * x) * (1 - window)
    bipolar_fold /= numpy.abs(bipolar_fold).max()
    
    with open("./dsp/fold_lut.h", 'w') as f:
        f.write("#ifndef FOLD_LUT_H_\n")
        f.write("#define FOLD_LUT_H_\n")

        f.write(f"const float fold_lut[{len(x)}] = {{\n")
        for i, el in enumerate(bipolar_fold):
            if (i != len(bipolar_fold)-1):
                f.write(f"{el:.6f}, ")
            else: f.write(f"{el:.6f}")
        f.write("\n};\n")

        f.write("#endif  // FOLD_LUT_H_\n")

if __name__ == "__main__":
    main()
