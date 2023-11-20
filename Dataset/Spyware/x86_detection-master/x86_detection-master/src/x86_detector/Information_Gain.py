import numpy as np

def getEntropy(D):
	#"""
	#Calculate and return entropy of 1-dimensional numpy array D 
	#"""
	length=D.size
	valueList=list(set(D))
	numVals=len(valueList)
	countVals=np.zeros(numVals)
	Ent=0
	for idx,val in enumerate(valueList):
		countVals[idx]=np.count_nonzero(D==val)
		Ent+=countVals[idx]*1.0/length*np.log2(length*1.0/countVals[idx])
	return Ent 

def getMaxInfoGain(D,X,feat=0):
	#""" 
	#Calculate maximum information gain w.r.t. the feature which is specified in column feat of the 1-dimensional array feature
	#"""
	EntWithoutSplit=getEntropy(D)
	feature=X[:,feat]
	length=len(feature)
	valueList=list(set(feature))
	splits=np.diff(valueList)/2.0+valueList[:-1]
	maxGain=0
	bestSplit=0
	bestPart1=[]
	bestPart2=[]
	for split in splits:
		Part1idx=np.argwhere(feature<=split)
		Part2idx=np.argwhere(feature>split)
		E1=getEntropy(D[Part1idx[:,0]])
		l1=len(Part1idx)
		E2=getEntropy(D[Part2idx[:,0]])
		l2=len(Part2idx)
		Gain=EntWithoutSplit-(l1*1.0/length*E1+l2*1.0/length*E2)
		if Gain > maxGain:
			maxGain=Gain
			bestSplit=split
			bestPart1=Part1idx
			bestPart2=Part2idx
	return maxGain,bestSplit,bestPart1,bestPart2
