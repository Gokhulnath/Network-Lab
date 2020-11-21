data=[]
file=open('out.tr','r')
for line in file:
	x=line.split(" ")
	x.pop(6)
	data.append(x)
nodes=[]
value=[]
drop_packets=0
print("From-to node \tPacket size(bytes)\tTime Taken(s)\tThroughput(Mbps)\tGoodput(Mbps)\n")
for a in data:
	if a[0]=="r":
		if int(a[2]+a[3]) not in nodes:
			nodes.append(int(a[2]+a[3]))
			temp=[]
			temp.append(int(a[2])) 	#from node
			temp.append(int(a[3]))		#To node
			temp.append(int(a[5]))		#packet size
			temp.append(float(a[1]))	#start time
			temp.append(float(a[1]))	#endtime
			if a[4]=="tcp" or a[4]=="cbr":
				temp.append(int(a[5]))	#for goodput data
			else:
				temp.append(0)
			value.append(temp)
		else:
			index=nodes.index(int(a[2]+a[3]))
			value[index][2]+=int(a[5])
			value[index][4]=float(a[1])
			if a[4]=="tcp" or a[4]=="cbr":
				value[index][5]+=int(a[5])
	if a[0]=="d":
		drop_packets+=1
for a in value:
	time_taken=a[4]-a[3]
	throughput=((a[2]*8)/time_taken)/1000000
	goodput=((a[5]*8)/time_taken)/1000000
	print("%2d-%2d\t\t%10d\t\t%5.8f\t%5.8f\t\t%5.8f"%(a[0],a[1],a[2],time_taken,throughput,goodput))
print("="*100)	
print("Droped Packets = ", drop_packets)
print("="*100)	
file.close()


'''
output:
gokhul@gokhul-ubuntu:~/Desktop/A8/Throughput,goodput,droppackets,congestion window$ python3 A8.py 
From-to node 	Packet size(bytes)	Time Taken(s)	Throughput(Mbps)	Goodput(Mbps)

 4- 1		    238200		2.50350000	0.76117436		0.76117436
 1- 2		    495310		2.88677900	1.37263019		1.32795756
 2- 7		    225720		2.57313700	0.70177375		0.70177375
 7- 2		      8400		2.47537700	0.02714738		0.00000000
 2- 1		    456680		2.83495000	1.28871409		1.26500996
 1- 4		      8400		2.48147300	0.02708069		0.00000000
 5- 2		    448280		2.75225700	1.30301785		1.30301785
 1- 6		    448280		2.76292300	1.29798767		1.29798767
 6- 1		    139280		2.76451700	0.40305051		0.35489744
 2- 5		     16120		2.67529000	0.04820412		0.00000000
 0- 1		    140070		2.49750000	0.44867267		0.44867267
 2- 3		    253260		2.48511300	0.81528687		0.81528687
====================================================================================================
Droped Packets =  67
====================================================================================================
'''
