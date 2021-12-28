import os
import pandas as pd
from torch.utils.data import Dataset
from torchvision.io import read_image
import numpy as np


class CustomDataset(Dataset):
    def __init__(self, tof_filename, cir_filename):
        tof_data = np.loadtxt(tof_filename, dtype=np.double)
        cir_data = np.loadtxt(cir_filename, dtype=np.double, delimiter=',')

        print(tof_data.shape)
        print(cir_data.shape)
        # self.labels = np.empty(shape=(len(cir_data), 1))
        self.labels = []
        self.data = []
        for i in range(cir_data.shape[0]):
            if cir_data[i][0] > 0.0:
                # self.labels[i] = tof_data[i]
                self.labels.append(tof_data[i])
                self.data.append(cir_data[i][0:150])

        self.labels = np.array(self.labels).reshape(-1, 1)
        self.labels = np.float32(self.labels)
        self.data = np.float32(self.data)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        return self.data[idx], self.labels[idx]
