import pandas as pd 
import numpy as np 

import matplotlib.pyplot as plt
import os 

def rename_files(folder):

    list_files = os.listdir(folder)
    png_files = [ ( 
            os.path.join( folder, f ) ,
            os.path.join( folder, f.replace("color_", "") )
        ) for f in list_files if f.endswith("png") ]

    for o, n in png_files:
        print (o, n)

        os.rename(o, n)


def normalize_quaternion(row):
    norm = np.sqrt(row['qw']**2 + row['qx']**2 + row['qy']**2 + row['qz']**2)
    print (norm)
    row['qw'] /= norm
    row['qx'] /= norm
    row['qy'] /= norm
    row['qz'] /= norm
    return row


fp1 = "/data/airsim_2/forwardCar/airsim.txt"
fp2 = "/data/airsim_2/backwardCar/airsim.txt"

df1 = pd.read_csv(fp1, delim_whitespace=True)
df2 = pd.read_csv(fp2, delim_whitespace=True)


print (df1.columns)


df1.to_csv( "/data/airsim_2/forwardCar/airsim_new.txt", sep=","  )
df1.to_csv( "/data/airsim_2/backwardCar/airsim_new.txt", sep=","  )

# df1.columns = ['x', 'y', 'z', 'qx', 'qy', 'qz', 'qw']
# df2.columns = ['x', 'y', 'z', 'qx', 'qy', 'qz', 'qw']

# # Apply normalization to each row
# # df1 = df1.apply(normalize_quaternion, axis=1)

# count = 1400
# plt.plot( df1['x'].to_numpy()[:count], df1['y'].to_numpy()[:count] , 'r-x')
# # plt.plot( df2['x'].to_numpy(), df2['y'].to_numpy() , 'b-x')
# plt.show()