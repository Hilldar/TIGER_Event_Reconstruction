import pickle
import sys
import numpy
import glob
def convert_pkl(filename):
    with open (filename, 'rb') as fin:
        data = pickle.load(fin)
    name,ext = filename.split(".")[0],filename.split(".")[1]
    with open (name+"_2."+ext, 'wb+') as fout:
        pickle.dump(data,fout, protocol=2)
    print (filename)
if __name__ == "__main__":
    for filename in glob.glob("CONF_run_???.pkl"):
        convert_pkl(filename)
