#This code snippet is for csv data seperating
import numpy as np
import csv
from sklearn.cluster import KMeans
for i in range(7,12):
    f = open("yellow_tripdata_2015-"+str(i)+".csv","rb")
    ff=open("yellow_tripdata_2015-"+str(i)+"-pickup.csv","w")
    fff=open("yellow_tripdata_2015-"+str(i)+"-dropoff.csv","w")
    reader=csv.reader(f,delimiter=',')
    #row_count = sum(1 for row in reader)
    #print(row_count)
    next(reader, None)
    for row in reader:
        ff.write(row[5]+","+row[6]+"\n")
        fff.write(row[9]+","+row[10]+"\n")
    #print sizeof(reader)
    f.close()
    ff.close()
    fff.close()
