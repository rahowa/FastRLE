import numpy as np
import pandas as pd 
from pathlib import Path
from typing import Union, List

from . import fast_rle_lib as fr


def about() -> None:
    """
    Helper function with base info
    """
    fr._about()


def read_csv(path: Union[str, Path]) -> List[fr.RleFile]:
    """
    Read .csv file with encoded RLE wtih following columns: \n
    "image_name", "rle", "image_shape"

    Parameters:
    -----------
        path: str | Path
        Path to .csv file
    
    Returns:
    --------
        csv file: RleFile
        result s
    """
    return fr._read_csv(str(path))


def decode(rles: List[fr.RleFile]) -> List[np.ndarray]:
    """
    Decode RLE files into masks

    Parameters:
    -----------
        rles: List[RleFiles]
        List of encoded masks in RLE encoding
    
    Returns:
    --------
        masks: List[ndarray]
        List of deocded masks
    """
    return fr._decode_rle(rles)


def encode(masks: List[np.ndarray]) -> List[fr.RleFile]:
    """
    Encode binary masks into RLE format

    Parameters:
    ----------
        masks: List[ndarray]
        List of binary masks

    Returns:
    --------
        rles: List[RleFiles]
        List of encoded rles
    """
    return fr._encode_rle(masks)


def save_csv(path: Union[str, Path], rles: List[fr.RleFile]) -> None:
    """
    Save RLE encodings into .csv file

    Parameters:
    -----------
        path: str | Path
        path to result .csv file

        rles: List[RleFile]
        list of encoded RLE files
    """
    fr._save_rle(str(path), rles)
