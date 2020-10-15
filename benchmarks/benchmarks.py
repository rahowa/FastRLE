import time
import numpy as np 
import pandas as pd

import fast_rle as fr
from functools import wraps

def measure(op_name):
    def measure_time(fun):
        @wraps(fun)
        def wrapper(*args, **kwargs):
            start = time.time()
            res = fun(*args, **kwargs)
            end = time.time()
            print(f"total time of [{op_name}] is {end - start} sec")
            return res
        return wrapper
    return measure_time


"""Original implementation at
https://www.kaggle.com/paulorzp/run-length-encode-and-decode
"""
def rle_encode(img):
    '''
    img: numpy array, 1 - mask, 0 - background
    Returns run length as string formated
    '''

    pixels = img.flatten()
    pixels = np.concatenate([[0], pixels, [0]])
    runs = np.where(pixels[1:] != pixels[:-1])[0] + 1
    runs[1::2] -= runs[::2]
    return ' '.join(str(x) for x in runs)
 

def rle_decode(mask_rle, shape):
    '''
    mask_rle: run-length as string formated (start length)
    shape: (height,width) of array to return 
    Returns numpy array, 1 - mask, 0 - background
    '''

    s = mask_rle.split()
    starts, lengths = [np.asarray(x, dtype=int) for x in (s[0:][::2], s[1:][::2])]
    starts -= 1
    ends = starts + lengths
    img = np.zeros(shape[0]*shape[1], dtype=np.uint8)
    for lo, hi in zip(starts, ends):
        img[lo:hi] = 1
    return img.reshape(shape)


@measure("Numpy rle decoding")
def np_rle_decode(rle_sizes):
    for rle, size in rle_sizes:
        tmp_res = rle_decode(rle, size)
    return tmp_res


@measure("FastRLE decoding")
def fast_rle_decode(parsed_csv):
    decoded_rles = fr.decode(parsed_csv)
    return decoded_rles


@measure("pandas CSV reading")
def pd_read_csv(filename: str = "assets/test_masks.csv"):
    df = pd.read_csv(filename)
    return df


@measure("FastRLE CSV reading")
def fast_rle_read_csv(filename: str = "assets/test_masks.csv"):
    parsed_csv = fr.read_csv(filename)
    return parsed_csv


def benchmark_decode():
    fr.about()
    df = pd_read_csv("../assets/test_masks.csv")
    parsed_csv = fast_rle_read_csv("../assets/test_masks.csv")

    rles = df["rle"]
    sizes = df['image_shape']
    sizes = [[int(size.split("x")[0]), int(size.split("x")[1])] for size in sizes]
    rle_sizes = zip(rles, sizes)
    np_rle_decode(rle_sizes)
    fast_rle_decode(parsed_csv)


if __name__ == "__main__":
    # benchmark_decode()

    print(fr.__spec__)