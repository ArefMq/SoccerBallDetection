#!/usr/bin/python
import mmlcore

with open("/home/cafebazaar/Workspace/newmanoid/SoccerBallDetection/interfaces/offline/build/test.img", 'r') as f:
    data = f.read()
    mmlcore.init()
    r = mmlcore.run(data)
    print r
