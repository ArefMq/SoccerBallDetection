#!/usr/bin/python
import json
from keras.models import Sequential
from keras.layers.core import Dense, Dropout
from keras.optimizers import sgd


# Brain Class
class Brain(object):
    IMAGE_SIZE = (128, 128)

    def __init__(self):
        self.model = None
        self.build_network()
        self.batch_size = 16
        self.epochs = 50
        self.data = None
        self.labels = None

    def build_network(self):
        self.model = Sequential()
        self.model.add(Dense(16384, input_dim=self.IMAGE_SIZE[0] * self.IMAGE_SIZE[1], activation='relu'))
        self.model.add(Dropout(0.5))
        self.model.add(Dense(128, activation='relu'))
        self.model.add(Dropout(0.5))
        self.model.add(Dense(64, activation='relu'))
        self.model.add(Dropout(0.5))
        self.model.add(Dense(1, activation='sigmoid'))
        self.model.compile(loss='binary_crossentropy', optimizer='rmsprop', metrics=['accuracy'])
        #		self.model = Sequential()
        #		self.model.add(Dense(hidden_size, input_shape=(imageSize[0] * imageSize[1],), activation='relu'))
        #		self.model.add(Dense(hidden_size, activation='relu'))
        #		self.model.add(Dense(hidden_size, activation='sigmoid'))
        #		self.model.add(Dense(1, activation='sigmoid'))
        #		self.model.compile(sgd(lr=.2), "mse")

    def train(self):
        self.model.fit(self.data, self.labels, batch_size=self.batch_size, epochs=self.batch_size)

    def predict(self):
        self.labels = self.model.predict(self.data, batch_size=1)
        return self.labels

    def load_data(self, data, labels):
        self.data = data
        self.labels = labels

    def load(self, filename):
        self.model.load_weights(filename + ".h5")

    def save(self, filename):
        self.model.save_weights(filename + ".h5", overwrite=True)
        with open(filename + ".json", "w") as outfile:
            json.dump(self.model.to_json(), outfile)
