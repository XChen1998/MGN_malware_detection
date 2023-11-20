import random
import math

def multiply(matrix,vector) :
	n = len(matrix.keys())
	key_list = matrix.keys()
	output = []
	for key1 in key_list :
		i = 0
		temp_sum = 0.0
		for key2 in key_list :
			temp_sum += float(matrix[key2][key1]*vector[i])
			i += 1	
		output.append(temp_sum)
					
	return output

def recursive_multiply(matrix,vector,no_of_times):
	if no_of_times == 1 :
		return multiply(matrix,vector)
	if no_of_times < 1 :
		return
	output = multiply(matrix,vector)
	return recursive_multiply(matrix,output,no_of_times - 1)

def compute_l2_norm(vector1,vector2) :
	i = 0
	sum_l2_norm = 0.0
	while i < len(vector1) :
		sum_l2_norm += (vector1[i] - vector2[i])*(vector1[i] - vector2[i])
		i += 1
	return math.sqrt(sum_l2_norm)

def compute_steady_state_dist(P,init_state,threshold) :
	l = 10
	n = l + random.randint(1,20)
	dist_l = recursive_multiply(P,init_state,l)
	dist_n = recursive_multiply(P,dist_l,n-l)
	no_of_iterations =  0
	while compute_l2_norm(dist_n,dist_l) > threshold :
		l += 1
		n += 1
		dist_l = multiply(P,dist_l)
		dist_n = multiply(P,dist_n)
		no_of_iterations += 1
		print "No of iterations = ", no_of_iterations
	
	return dist_n

#matrix = {}
#matrix[10] = {}
#matrix[200] = {}
#matrix[205] = {}
#matrix[10][10] = 0.23
#matrix[10][200] = 0.34
#matrix[10][205] = 0.43
#matrix[200][10] = 0.12
#matrix[200][200] = 0.56
#matrix[200][205] = 0.32
#matrix[205][10] = 0.43
#matrix[205][200] = 0.23
#matrix[205][205] = 0.34

#init_state = [0,1,0]
#print multiply(matrix,init_state)
#print matrix.keys()
#steady_state_dist = compute_steady_state_dist(matrix,init_state,0.001)
#print "steady state dist = ", steady_state_dist

#print multiply(matrix,steady_state_dist)

