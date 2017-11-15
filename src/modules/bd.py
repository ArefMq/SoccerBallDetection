#!/usr/bin/python

import sys
import json
import numpy as np
import cv2
import zmq
import time

from keras.models import Sequential
from keras.layers.core import Dense, Dropout
from keras.optimizers import sgd

from os import listdir
from os.path import isfile, join

#-- Constants
imageSize = (128, 128)
hidden_size=20000
dataset_root_dir = "./dataset"

network_income_port = 9000
network_delivery_port = 9001
network_protocol = "tcp"
network_masked_ip = '127.0.0'#"192.168.14"

#-- Functions

	
def recieveImage(listener):
	rc = listener.recv()
	buf = buffer(rc)
	rc = np.frombuffer(buf, dtype='uint8')
	rc = list(rc)
	rc = np.reshape(rc, (128, 128))
	rc = rc.astype('uint8')
	return rc









		

















#-- Main Function
if __name__ == "__main__":
	brain = Brain()
	
	if (len(sys.argv) > 1 and sys.argv[1] == 'train'):
		print "running in train mode"
		if (len(sys.argv) > 2):
			filename = sys.argv[2]
		else:
			filename = "model"
		print "model : ", filename
		brain.loadData()
		brain.train()
		brain.save('')
	elif len(sys.argv) > 1 and sys.argv[1] == 'help':
		print "Usage: " + sys.argv[0] + " [train | test | socket | collect] [model_name]\n"

	elif len(sys.argv) > 1 and sys.argv[1] == 'collect':
		print "runing in collection mode"
		ctx = zmq.Context.instance()
		listener = ctx.socket(zmq.REP)
		listener.connect("{0}://{1}.{2}:{3}".format(network_protocol, network_masked_ip, '1', network_income_port))

		#if (len(sys.argv) > 2):
		#	filename = sys.argv[2]
		#else:
		#	filename = "model"
		#brain.load(filename)

		print "socket ready"
		#listener.setsockopt(zmq.SUBSCRIBE, b'')
		r = 0
		while True:
			# Recieveing Data
			rc = recieveImage(listener);
			rc = cv2.resize(rc, (300, 300), interpolation=0)
			
			# Save recieved data
			p = './data/frame_' + str(time.time()) + ".png"
			cv2.imwrite(p, rc)
			
			# Send responce
			p = str(r)
			listener.send_string(p)
			
			cv2.imshow("img", rc)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break
			r = 1 - r

		listener.close(linger=0)
		ctx.term()
		
	elif len(sys.argv) > 1 and sys.argv[1] == 'socket':
		print "runing in socket mode"
		ctx = zmq.Context.instance()
		listener = ctx.socket(zmq.REP)
		listener.connect("{0}://{1}.{2}:{3}".format(network_protocol, network_masked_ip, '1', network_income_port))

		#if (len(sys.argv) > 2):
		#	filename = sys.argv[2]
		#else:
		#	filename = "model"
		#brain.load(filename)

		print "socket ready"
		#listener.setsockopt(zmq.SUBSCRIBE, b'')
		r = 0
		while True:
			rc = recieveImage(listener);
			rc = cv2.resize(rc, (300, 300), interpolation=0)
			
			cv2.imshow("img", rc)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				break
			
			listener.send(r)
			r = 1 - r
			#print brain.predict()

		listener.close(linger=0)
		ctx.term()

	else:
		print "runing in default mode"
		if (len(sys.argv) > 1):
			filename = sys.argv[1]
		else:
			filename = "model"
		print "model : ", filename
		brain.load(filename)
		print brain.predict()

		
