#!/usr/bin/python

import cv2
from Brain import Brain
from os import listdir
import numpy as np

imageSize = Brain.IMAGE_SIZE


# tools
def read_image(path, scale_size=imageSize):
    img = cv2.resize(cv2.imread(path, 0), scale_size)
    d = np.asarray(img)
    d = d.reshape((1, scale_size[0] * scale_size[1]))
    return d


class Core(object):
    def __init__(self):
        self.brain = Brain()
        self.mode = "prediction"

    def load_data(self, dataset_root_dir):
        self.mode = "train"

        # -- Scaning dataset directory
        dataset_pos = dataset_root_dir + "_pos"
        dataset_neg = dataset_root_dir + "_neg"
        # [TODO] : check if the directory exists
        onlyfiles_pos = [f for f in listdir(dataset_pos) if isfile(join(dataset_pos, f))]
        onlyfiles_neg = [f for f in listdir(dataset_neg) if isfile(join(dataset_neg, f))]

        # -- Initializing data
        datasetSize = len(onlyfiles_pos) + len(onlyfiles_neg)
        self.brain.data = np.zeros((datasetSize, imageSize[0] * imageSize[1]))
        self.brain.labels = np.zeros((datasetSize, 1))

        # -- Loading files
        for i in range(0, len(onlyfiles_pos)):
            f = dataset_pos + "/" + onlyfiles_pos[i]
            print "reading : ", f
            self.brain.data[i] = read_image(f, imageSize)
            self.brain.labels[i] = 1
        offset = len(onlyfiles_pos)

        for i in range(0, len(onlyfiles_neg)):
            f = dataset_neg + "/" + onlyfiles_neg[i]
            print "reading : ", f
            self.brain.data[i + offset] = read_image(f, imageSize)
            self.brain.labels[i + offset] = 0

    def train(self):
        if self.mode != "train":
            raise Exception("can not use train in modes other than 'train'")
        self.brain.train()
        self.brain.save('model')

    def predict_ball(self, data):
        if self.mode != "prediction":
            raise Exception("can not use prediction in modes other than 'prediction'")
        self.brain.data = data
        self.brain.predict()
        result = self.brain.labels[0]
        return result


def init(mode="prediction"):
    global core
    core = Core()

    if mode == "train":
        core.load_data("./dataset")
        core.train()
    elif mode == "prediction":
        pass
    else:
        raise Exception("undefined behavior")


def run(chars):
    global core
    res = np.fromstring(chars, dtype=np.uint8)
    res = res.reshape((1, -1))
    return core.predict_ball(res[0:1, :])[0]
