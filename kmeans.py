import sys

import numpy as np
from pyspark import SparkContext
from pyspark.mllib.clustering import KMeans
from pyspark import SparkConf
def parseVector(line):
    return np.array([float(x) for x in line.split(',')])


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print >> sys.stderr, "Usage: kmeans <file> <k>"
        exit(-1)
    conf = SparkConf().setAppName("KMeans").setMaster("local").set("spark.executor.memory", "100g")
    sc = SparkContext(conf=conf)
    #sc = SparkContext(appName="KMeans").setMaster("local").set("spark.executor.memory", "100g")

    lines = sc.textFile(sys.argv[1])
    data = lines.map(parseVector)
    k = int(sys.argv[2])
    model = KMeans.train(data, k)
    print "Final centers: " + str(model.clusterCenters)
    model.clusterCenters.foreach(println)
    #f=open(sys.argv[1]+"-k="+sys.argv[2]+"doubleresult.txt","wb");
    #centers = model.clusterCenters()
    #for center in centers:
    #    print(center)
    model.save("test.txt")
    f=open("sendResult.txt","wb");
    f.write(str(model.clusterCenters))
    #f.write("Final centers: " + str(model.clusterCenters))
    f.close()
    ff=open("/var/www/html/sendResult.txt","wb")
    ff.write(str(model.clusterCenters))
    ff.close()
    sc.stop()
