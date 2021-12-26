import os
import pandas as pd
from torch.utils.data import Dataset
from torchvision.io import read_image
import numpy as np


class CustomDataset(Dataset):
    def __init__(self, num):
        self.labels = np.random.rand(num, 1) * 100
        self.labels = np.float32(self.labels)
        self.data = np.random.rand(num, 10) * 100
        self.data = np.float32(self.data)

    def __len__(self):
        return len(self.labels)

    def __getitem__(self, idx):
        return self.data[idx], self.labels[idx]
