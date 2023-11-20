
import os
import re
import binascii
import random as rnd
import math
import numpy as np
from sklearn.svm import SVC
import pickle
from sklearn.externals import joblib
from distorm3 import Decode, Decode16Bits, Decode32Bits, Decode64Bits
import shutil
import sys
import time
from steady_state_dist import *
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.backends.backend_tkagg
import FileDialog
import sklearn
from scipy import *
import distorm3
from Information_Gain import *
from sklearn.tree import DecisionTreeClassifier
from sklearn.feature_selection import RFECV
from sklearn.cross_validation import StratifiedKFold


MAX_NO_OF_HITS = 0
no_of_instructions_to_check = 0
Block_size = 0
no_of_rounds = 0
Entropy_Block_size = 0
Entropy_Block_size_start = 0
Entropy_Block_size_end = 0
step_size = 0
Data_transfer_instructions = {}
Control_flow_instructions = {}
no_of_control_flow_instructions = 0
no_of_data_transfer_instructions = 0
Valid_Prefix = {}

# For simulating experiments on a dataset
training_files_directory = ''
testing_files_directory = ''
output_directory = ''
dataset_directory = ''
valid_extensions = []
invalid_extensions = []
weighted_avg_time = 0.0
avg_time = 0.0
total_file_length = 0.0
total_no_of_train_samples = 0
total_no_of_test_samples = 0
total_no_of_packets = 0



# For on the fly testing
path_to_classifier = ''



def x86_detector_init() :


	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions


	MAX_NO_OF_HITS = 200
	no_of_instructions_to_check = 10
	Block_size = int(256/no_of_instructions_to_check)
	no_of_rounds = 1
	Entropy_Block_size = 256
	Entropy_Block_size_start = 256
	Entropy_Block_size_end = 256
	step_size = 0

		
	Control_flow_instructions[1] = 'ff'
	Control_flow_instructions[2] = '0f80'
	Control_flow_instructions[3] = '0f81'
	Control_flow_instructions[4] = '0f82'
	Control_flow_instructions[5] = '0f83'
	Control_flow_instructions[6] = '0f84'
	Control_flow_instructions[7] = '0f85'
	Control_flow_instructions[8] = '0f86'
	Control_flow_instructions[9] = '0f87'
	Control_flow_instructions[10] = '0f88'
	Control_flow_instructions[11] = '0f89'
	Control_flow_instructions[12] = '0f8a'
	Control_flow_instructions[13] = '0f8b'
	Control_flow_instructions[14] = '0f8c'
	Control_flow_instructions[15] = '0f8d'
	Control_flow_instructions[16] = '0f8e'
	Control_flow_instructions[17] = '0f8f'
	Control_flow_instructions[18] = '9a'
	Control_flow_instructions[19] = 'c2'
	Control_flow_instructions[20] = 'c3'
	Control_flow_instructions[21] = 'ca'
	Control_flow_instructions[22] = 'cb'
	Control_flow_instructions[23] = 'e0'
	Control_flow_instructions[24] = 'e1'
	Control_flow_instructions[25] = 'e2'
	Control_flow_instructions[26] = 'e8'
	Control_flow_instructions[27] = 'e9'
	Control_flow_instructions[28] = 'eb'
	no_of_control_flow_instructions = 28

	Valid_Prefix['Instruction Prefix'] = ['f0']
	Valid_Prefix['Address Override Prefix'] = ['67']
	Valid_Prefix['Operand Override Prefix'] = ['66']
	Valid_Prefix['Segment Override Prefix'] = ['2e','36','3e','26','64','65']

	Data_transfer_instructions[1] = '86'
	Data_transfer_instructions[2] = '87'
	Data_transfer_instructions[3] = '88'
	Data_transfer_instructions[4] = '89'
	Data_transfer_instructions[5] = '8a'
	Data_transfer_instructions[6] = '8b'
	Data_transfer_instructions[7] = '8c'
	Data_transfer_instructions[8] = '8d'
	Data_transfer_instructions[9] = '8e'
	Data_transfer_instructions[10] = '91'
	Data_transfer_instructions[11] = '92'
	Data_transfer_instructions[12] = '93'
	Data_transfer_instructions[13] = '94'
	Data_transfer_instructions[14] = '95'
	Data_transfer_instructions[15] = '96'
	Data_transfer_instructions[16] = '97'
	Data_transfer_instructions[17] = '98'
	Data_transfer_instructions[18] = '99'
	Data_transfer_instructions[19] = 'a0'
	Data_transfer_instructions[20] = 'a1'
	Data_transfer_instructions[21] = 'a2'
	Data_transfer_instructions[22] = 'a3'
	Data_transfer_instructions[23] = 'a4'
	Data_transfer_instructions[24] = 'aa'
	Data_transfer_instructions[25] = 'ab'
	Data_transfer_instructions[26] = 'ac'
	Data_transfer_instructions[27] = 'ad'
	Data_transfer_instructions[28] = 'b0'
	Data_transfer_instructions[29] = 'b1'
	Data_transfer_instructions[30] = 'b2'
	Data_transfer_instructions[31] = 'b3'
	Data_transfer_instructions[32] = 'b4'
	Data_transfer_instructions[33] = 'b5'
	Data_transfer_instructions[34] = 'b6'
	Data_transfer_instructions[35] = 'b7'
	Data_transfer_instructions[36] = 'b8'
	Data_transfer_instructions[37] = 'b9'
	Data_transfer_instructions[38] = 'ba'
	Data_transfer_instructions[39] = 'bb'
	Data_transfer_instructions[40] = 'bc'
	Data_transfer_instructions[41] = 'bd'
	Data_transfer_instructions[42] = 'be'
	Data_transfer_instructions[43] = 'bf'
	no_of_data_transfer_instructions = 43

	valid_extensions = ['exe','EXE','dll','DLL','zip','ZIP']
	invalid_extensions = ['bin','c']

		
def add_control_flow_instruction(opcode) :

	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	no_of_control_flow_instructions += 1
	Control_flow_instructions[no_of_control_flow_instructions] = opcode

def add_data_transfer_instruction(opcode) :

	global Data_transfer_instructions 
	global no_of_data_transfer_instructions
	no_of_data_transfer_instructions += 1
	Data_transfer_instructions[no_of_data_transfer_instructions] = opcode

def set_path_to_output(path) :
	global output_directory
	global training_files_directory
	global testing_files_directory

	output_directory = path
	training_files_directory = output_directory + "/training_files"
	testing_files_directory = output_directory + "/test_files"


def set_path_to_dataset(path) :
	global dataset_directory 
	dataset_directory = path

def set_path_to_classifier(path) :
	global path_to_classifier
	path_to_classifier = path

def set_no_of_instructions_to_sample(count) :
	global no_of_instructions_to_check
	no_of_instructions_to_check = count

def set_Entropy_Block_size(size) :
	global Entropy_Block_size 
	Entropy_Block_size = size


def init_experiment(dataset_directory,output_directory,path_to_classifier) :

	set_path_to_dataset(dataset_directory)
	set_path_to_output(output_directory)
	set_path_to_classifier(path_to_classifier)

def simulate_experiment(no_of_iterations) :

	global dataset_directory
	global output_directory
	global training_files_directory
	global testing_files_directory
	global path_to_classifier

	if dataset_directory == "" :
		print "Path to Data set is not set. Cannot Simulate. Exiting"
		exit()
	if output_directory == "" :
		print "Output directory is not set. Cannot Simulate. Exiting"
		exit()

	if path_to_classifier == "" :
		print "Path to classifier is not set. Cannot Simulate. Exiting"
		exit()

	
	iteration_counter = 0
	while iteration_counter < no_of_iterations :
		print "Creating training and test set from the specified Data set"
		if not os.path.exists(training_files_directory):
			os.makedirs(training_files_directory)
		else :
			shutil.rmtree(training_files_directory) 
			print "Training files directory cleaned up"
			os.makedirs(training_files_directory)

		if not os.path.exists(testing_files_directory):
		    os.makedirs(testing_files_directory)
		else :
			shutil.rmtree(testing_files_directory) 
			print "Testing files directory cleaned up"
			os.makedirs(testing_files_directory)				

		for root, dirs, files in os.walk(dataset_directory):
			for file in files :
				prob = rnd.uniform(0,10)
				path = os.path.join(root, file)
				path = path.replace('\\','/')
				if prob <= 3 : # add it to the training directory
					shutil.copy2(path,training_files_directory)
				else :
					shutil.copy2(path,testing_files_directory)					

		print "Training and Test set created"

	
		train()
		test_for_experiment()
		iteration_counter += 1


def check_prefix_existance(curr_position,byte_seq_string,prefixes_to_check):
	start_of_instruction = curr_position
	current_state = 0
	if curr_position == 0:
		return start_of_instruction
		
	if len(prefixes_to_check) == 0 :
		return curr_position
		
	elif byte_seq_string[curr_position-1] in Valid_Prefix[prefixes_to_check[0]] :
		prefixes_to_check.remove(prefixes_to_check[0])
		start_of_instruction = check_prefix_existance(curr_position - 1, byte_seq_string,prefixes_to_check)
		return start_of_instruction
	else :
		prefixes_to_check.remove(prefixes_to_check[0])
		start_of_instruction = check_prefix_existance(curr_position,byte_seq_string,prefixes_to_check)
		return start_of_instruction
		
				
def get_positions_of_occurance(key_list,input_string) :
	position_list = {}
	start = 0
	stop = len(input_string)
	for key in key_list :
		position_list[key] = []
		
	while start < stop :
		if start < len(input_string) -1 :
			if input_string[start:start + 2] in key_list or input_string[start:start+4] in key_list :
				if input_string[start:start+2] in key_list :				
					position_list[input_string[start:start +2]].append(start)
				else :
					position_list[input_string[start:start +4]].append(start)
		start = start + 2
	
	return position_list		
	

def add_lists(list1,list2) :
	i = 0
	sum_list = [0,0,0,0,0,0,0,0,0,0]
	while len(list1) == len(list2) and i < len(list1) :
		sum_list[i] = list1[i] + list2[i]
		i = i + 1		
	return sum_list


def hist(source):
	hist = {}; l = 0;
	start = 0
	stop = len(source)
	while start < stop :
		l += 1
		if source[start:start+2] not in hist.keys():
			hist[source[start:start + 2]] = 0
		hist[source[start:start + 2]] += 1
		start = start + 2
	return (l,hist)
 
def entropy(hist,l):
	elist = []
	#print 'Histogram',hist,' length ', l, 'hist values', hist.values()
	for v in hist.values():
		
		c = float(v) /float(l)
		if c != 0.0 :
			elist.append(-c * math.log(c ,2))
	#print 'elist', elist
	return sum(elist)
def obtain_low_entropy_blocks(input_string,Entropy_Block_size) :
	entropy_threshold = 3.0
	start = 0
	stop = len(input_string)
	suspected_block_list = []
	while start < stop :
		if start + Entropy_Block_size >= stop :
			(l,h) = hist(input_string[start:stop])
		else :
			(l,h) = hist(input_string[start:start + Entropy_Block_size])
		if entropy(h,l) <= entropy_threshold :
			suspected_block_list.append(start)
		#print 'Entropy', entropy(h,l)
		start = start + Entropy_Block_size
	#print 'Suspected block list', suspected_block_list	
	return suspected_block_list
	


def compute_entropy(input_string,Entropy_Block_size) :
	entropy_threshold = 6.0
	start = 0
	stop = len(input_string)
	suspected_block_list = []
	while start < stop :
		if start + Entropy_Block_size >= stop :
			(l,h) = hist(input_string[start:stop])
		else :
			(l,h) = hist(input_string[start:start + Entropy_Block_size])
		if entropy(h,l) > entropy_threshold :
			suspected_block_list.append(start)
		#print 'Entropy', entropy(h,l)
		start = start + Entropy_Block_size
	#print 'Suspected block list', suspected_block_list	
	return suspected_block_list
		
		
def obtain_positions_of_occurance(input_string,list_of_keys) :
	list_of_positions = []
	Instruction_count_dict = {}
	start = 0
	stop = len(input_string)
	while start < stop :
		if start < len(input_string) -1 :
			if input_string[start:start + 2] in list_of_keys  :				
				if input_string[start:start + 2] in Instruction_count_dict.keys() :
					Instruction_count_dict[input_string[start:start + 2]] = Instruction_count_dict[input_string[start:start+2]] + 1
				else :
					Instruction_count_dict[input_string[start:start + 2]] = 1
				list_of_positions.append(start)
			elif input_string[start:start + 4] in list_of_keys  :				
				if input_string[start:start + 4] in Instruction_count_dict.keys() :
					Instruction_count_dict[input_string[start:start + 4]] = Instruction_count_dict[input_string[start:start+4]] + 1
				else :
					Instruction_count_dict[input_string[start:start + 4]] = 1
				list_of_positions.append(start)
		start = start + 2
	Instruction_count_list = Instruction_count_dict.values()
	return [list_of_positions,Instruction_count_list]

def compute_mean(input_list) :
	total_sum = 0.0
	start = 0
	while start < len(input_list) :
		total_sum = total_sum + input_list[start]
		start = start + 1
	if len(input_list) != 0 :
		return float(start/len(input_list))
	else :
		return 0.0
		
def compute_variance(input_list) :
	if len(input_list) != 0 :
		mean = compute_mean(input_list) 
		total_sum = 0.0
		for entry in input_list :
			total_sum = total_sum + (entry*entry)
		total_sum = float(total_sum/len(input_list))
		variance = total_sum - (mean*mean)
		return variance
	else :
		return 0
	

def compute_closeness_measure(list_of_positions,Block_size) :
	transformed_distance_list =[]
	transformed_distance_count = {}
	closeness_measure = 0.0
	start = 1
	stop = len(list_of_positions)
	
	while start < stop :
		transformed_distance = int((list_of_positions[start] - list_of_positions[start-1])/Block_size) + 1
		transformed_distance_list.append(transformed_distance)
		if transformed_distance in transformed_distance_count.keys() :
			transformed_distance_count[transformed_distance] = transformed_distance_count[transformed_distance] + 1
		else :
			transformed_distance_count[transformed_distance] = 1
		start = start + 1
	
	
	for key in transformed_distance_count.keys() :
		closeness_measure = closeness_measure + (float(transformed_distance_count[key])/(float(key*len(list_of_positions))))
		


	return [closeness_measure,transformed_distance_list]

def inverse_transform_sampling(pdf) :
	r = rnd.uniform(0, 1)
	s = 0.0
	for item, prob in pdf:
		s += prob
		if s >= r:
			return item
	return pdf[-1]


def compute_harm_factor(Sig_CF_Blocks,P) :
	
	NO_OF_ITERATIONS = 200
	i = 0
	total_harm_factor = 0
	Sig_CF_Block_IDs = Sig_CF_Blocks.keys()
	if len(Sig_CF_Block_IDs) > 1 :
		initial_state = rnd.randint(0,len(Sig_CF_Block_IDs)-1)
	else :
		initial_state = 0
	current_state = initial_state
	current_Block_ID = Sig_CF_Block_IDs[current_state]

	
	#sum_weight = 0
	#for entry in Sig_CF_Blocks.keys() :
	#	sum_weight += len(Sig_CF_Blocks[entry])
	#while i < NO_OF_ITERATIONS :
		
	#	pdf = []
	#	for Block_ID in Sig_CF_Block_IDs :
	#		pdf.append((Block_ID,P[current_Block_ID][Block_ID]))
			
	#	next_Block_ID = inverse_transform_sampling(pdf)
		##total_harm_factor += len(Sig_CF_Blocks[next_Block_ID]) + sum_weight
	#	total_harm_factor += len(Sig_CF_Blocks[next_Block_ID])*len(Sig_CF_Blocks.keys())
	#	i += 1

	init_state = []
	i = 0
	while i < len(Sig_CF_Block_IDs) :
		init_state.append(0)
		i += 1
	init_state[current_state] = 1
	steady_state_dist = compute_steady_state_dist(P,init_state,0.001)
	state_list = P.keys()
	
	i = 0
	total_harm_factor = 0.0
	while i < len(state_list) :
		total_harm_factor += steady_state_dist[i]*len(Sig_CF_Blocks[state_list[i]])*len(Sig_CF_Blocks.keys())
		#total_harm_factor += steady_state_dist[i]*len(Sig_CF_Blocks[state_list[i]])
		i += 1
	return total_harm_factor
				

def compute_limiting_rw_harm_factor(Sig_CF_Blocks) :
	
	NO_OF_ITERATIONS = 200
	i = 0
	total_harm_factor = 0.0
		
	sum_weight = 0
	for entry in Sig_CF_Blocks.keys() :
		sum_weight += len(Sig_CF_Blocks[entry])
	
	for curr_block in Sig_CF_Blocks.keys() :
		block_contribution = len(Sig_CF_Blocks[curr_block])
		block_weight =  float(block_contribution)/float(sum_weight)
		total_harm_factor += float(block_weight) * float(block_contribution)*len(Sig_CF_Blocks.keys())
				
	return total_harm_factor
				
	

def compute_transition_matrix(Sig_CF_Blocks) :
	P = {}
	
	for key in Sig_CF_Blocks.keys() :
		P[key] = {}
		
	
	for source_Block_no in Sig_CF_Blocks.keys() :
		total_length = 0.0

		for Dest_Block_no in Sig_CF_Blocks.keys() :
			#total_length += float(len(Sig_CF_Blocks[source_Block_no])+len(Sig_CF_Blocks[Dest_Block_no]))
			total_length += float(len(Sig_CF_Blocks[Dest_Block_no]))
		for Dest_Block_no in Sig_CF_Blocks.keys() :
			#P[source_Block_no][Dest_Block_no] = float(len(Sig_CF_Blocks[source_Block_no])+len(Sig_CF_Blocks[Dest_Block_no]))/float(total_length)
			P[source_Block_no][Dest_Block_no] = float(len(Sig_CF_Blocks[Dest_Block_no]))/float(total_length)

	#print "Transition matrix = ", P
	return P
	
	


def return_largest_CF_Block(CF_Blocks) :
	largest_CF_Block_len = 0
	largest_CF_Block_ID = -1
	for key in CF_Blocks.keys() :
		if len(CF_Blocks[key]) >= largest_CF_Block_len :
			largest_CF_Block_len = len(CF_Blocks[key])
			largest_CF_Block_ID = key
	return largest_CF_Block_ID


def obtain_most_significant_control_flow_blocks(CF_Blocks_arg) :

	MAX_NO_OF_SIGNIFICANT_CF_BLOCKS = 5
	Significant_CF_Block_IDs = []
	
	CF_Blocks = {}
	CF_Blocks = CF_Blocks_arg
	
	i = 0
	while i < MAX_NO_OF_SIGNIFICANT_CF_BLOCKS :
		next_Block_ID = return_largest_CF_Block(CF_Blocks)
		if next_Block_ID == -1 :
			break
		else :
			Significant_CF_Block_IDs.append(next_Block_ID)
			del CF_Blocks[next_Block_ID]
		i += 1
			
	return Significant_CF_Block_IDs
	

def obtain_control_flow_blocks(DT_instruction_positions,CF_instruction_positions) :
	
	CF_Blocks = {}
	Block_no = 1
	i = 0
	for CF_pos in CF_instruction_positions :
		#CF_Blocks[Block_no] = [1]
		CF_Blocks[Block_no] = []
		
		while i < len(DT_instruction_positions) and DT_instruction_positions[i] < CF_pos :
			CF_Blocks[Block_no].append(DT_instruction_positions[i])
			i += 1
		if i == len(DT_instruction_positions) :
			return CF_Blocks
		Block_no += 1
	return CF_Blocks

def obtain_harm_factor(file_data_string) :

	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global training_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory


	avg_non_sig_weight = 0.0
	no_of_non_sig_blocks = 0
	Instruction_set = Data_transfer_instructions.values()
	Check_set = Instruction_set
	#Check_set = rnd.sample(Instruction_set,20)

	[DT_instruction_positions,Instruction_count_list] = obtain_positions_of_occurance(file_data_string,Check_set)

	[CF_instruction_positions,Instruction_count_list] = obtain_positions_of_occurance(file_data_string,Control_flow_instructions.values())
	
	CF_Blocks_copy = obtain_control_flow_blocks(DT_instruction_positions,CF_instruction_positions)
	
	CF_Blocks = dict(CF_Blocks_copy)
	Sig_CF_Block_IDs = obtain_most_significant_control_flow_blocks(CF_Blocks_copy)

	#NC = len(CF_Blocks.keys())
	NC = 0
	
	NO_OF_ITERATIONS = 200
	MAX_NO_OF_SIGNIFICANT_CF_BLOCKS = 5

	if len(CF_instruction_positions) == 0 or len(DT_instruction_positions) == 0 : 
		if len(DT_instruction_positions) == 0 and len(CF_instruction_positions) != 0:
			harm_factor = 0  #did not exist before#			

			if len(CF_instruction_positions) >= MAX_NO_OF_SIGNIFICANT_CF_BLOCKS :
				#harm_factor = 1200 + 1*200 #first part + non_sig_weight part				
				#harm_factor = (MAX_NO_OF_SIGNIFICANT_CF_BLOCKS + 1)*NO_OF_ITERATIONS + 1*NO_OF_ITERATIONS
				#was not commented#harm_factor = (MAX_NO_OF_SIGNIFICANT_CF_BLOCKS + 1) + 1
				#was not commented#lim_harm_factor = 1*(MAX_NO_OF_SIGNIFICANT_CF_BLOCKS + 1)
				pass
			else:
				#harm_factor = len(CF_instruction_positions + 1)*NO_OF_ITERATIONS
				#was not commented#harm_factor = len(CF_instruction_positions + 1)
				#was not commented#lim_harm_factor = 1*len(CF_instruction_positions + 1)
				pass
			
		elif len(CF_instruction_positions) == 0 and len(DT_instruction_positions) != 0 :
			#harm_factor = len(DT_instruction_positions)*NO_OF_ITERATIONS*2
			#was not commented#harm_factor = len(DT_instruction_positions)*2
			harm_factor = len(DT_instruction_positions)*2
			lim_harm_factor = len(DT_instruction_positions)*2
			
		else :
			harm_factor = 1
	else :
		Sig_CF_Blocks = {}
		new_hf_factor = 0.0
		new_total_weight = 0.0
		
		for key in CF_Blocks.keys() :
			new_hf_factor += float(len(CF_Blocks[key]))*float(len(CF_Blocks[key]))
			new_total_weight += len(CF_Blocks[key]) 
			if len(CF_Blocks[key]) != 0  :
				NC += 1
	
			if key in Sig_CF_Block_IDs :
				Sig_CF_Blocks[key] = CF_Blocks[key]
			else :
				avg_non_sig_weight += float(len(CF_Blocks[key]))*float(len(CF_Blocks[key]))
				no_of_non_sig_blocks += len(CF_Blocks[key])
		if avg_non_sig_weight != 0.0 :
			avg_non_sig_weight = float(avg_non_sig_weight)/float(no_of_non_sig_blocks)
		

		##print Sig_CF_Blocks,Sig_CF_Block_IDs,CF_Blocks.keys()
		#was not commented#PT_matrix = compute_transition_matrix(Sig_CF_Blocks)
		###harm_factor = compute_harm_factor(Sig_CF_Blocks,PT_matrix)*(len(Sig_CF_Block_IDs) + 1)
		#harm_factor = (compute_harm_factor(Sig_CF_Blocks,PT_matrix) + avg_non_sig_weight*NO_OF_ITERATIONS)


		#was not commented#harm_factor = (compute_harm_factor(Sig_CF_Blocks,PT_matrix)) + avg_non_sig_weight
		#was not commented#harm_factor = (compute_harm_factor(Sig_CF_Blocks,PT_matrix))
		if new_hf_factor != 0.0 :
			new_hf_factor = float(NC*new_hf_factor)/float(new_total_weight)

		harm_factor = new_hf_factor

		#lim_harm_factor = compute_limiting_rw_harm_factor(Sig_CF_Blocks)

		
		
	#return math.log(harm_factor) - math.log(NO_OF_ITERATIONS)
	return math.log(harm_factor + 1)
	#return math.log(lim_harm_factor)

def compute_no_of_control_flow_hits(file_data_string) :

	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global training_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory
	

	no_of_hits = 0
	total_no_of_occurances = 0.0

	position_of_occurance = get_positions_of_occurance(Control_flow_instructions.values(),file_data_string)
		
		
	#-------------------------------------- Compute No of Hits-------------------------------------------------------------------#
	for key in Control_flow_instructions.values() :
		total_no_of_occurances = total_no_of_occurances + len(position_of_occurance[key])
					
		for position in position_of_occurance[key] :
	
			prefixes_to_check = ['Segment Override Prefix', 'Operand Override Prefix', 'Address Override Prefix', 'Instruction Prefix']
			potential_start_of_instruction = check_prefix_existance(position, file_data_string,prefixes_to_check = prefixes_to_check)
			if potential_start_of_instruction > 31 : 
				start = potential_start_of_instruction - 32
				stop = potential_start_of_instruction
				
				while start < stop :
					decoded_instructions_in_window = Decode(0x400000, file_data_string[start:stop].decode('hex'), Decode32Bits)
					last_instruction = decoded_instructions_in_window[len(decoded_instructions_in_window) - 1][2] # To access the instruction mnemonic
						
					if last_instruction.startswith('DB') :
						pass
					else :
						no_of_hits = no_of_hits + 1
						break
					start = start + 2

			else :
				if potential_start_of_instruction % 2 == 1 :
					start = 1
				else :
					start = 0
				stop = potential_start_of_instruction
				while start < stop :
					decoded_instructions_in_window = Decode(0x400000, file_data_string[start:stop].decode('hex'), Decode32Bits)
					last_instruction = decoded_instructions_in_window[len(decoded_instructions_in_window) - 1][2] # To access the instruction mnemonic
						
					if last_instruction.startswith('DB'):
						pass
					else :
						no_of_hits = no_of_hits + 1
						break
					start = start + 2
			if no_of_hits > MAX_NO_OF_HITS :
				break
			
		if no_of_hits > MAX_NO_OF_HITS :
			break
	#----------------------------------------------------------------------------------------------------------------------------#

	return no_of_hits

def extract_features(file_data_string) :

	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global training_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory

	# No of control flow hits	
	no_of_hits = compute_no_of_control_flow_hits(file_data_string)
	
	# Harm factor
	
	min_harm_factor = 1000000
	i = 0 
	while i < 1 :
		harm_factor = obtain_harm_factor(file_data_string)
		if min_harm_factor > harm_factor :
			min_harm_factor = harm_factor
		i += 1

	harm_factor = min_harm_factor
	
	print "Harm factor = ", harm_factor

	
			
				
	#--------------------------Compute Closeness measure for Data transfer instructions------------------------------------------#

	Instruction_set = Data_transfer_instructions.values()
	avg_closeness_measure = 0.0
	max_closeness_measure = 0.0
	counter = 0
	no_of_tries = 1
	while counter < no_of_tries :
		Check_set = rnd.sample(Instruction_set,no_of_instructions_to_check)
		#print Check_set
		[list_of_positions,Instruction_count_list] = obtain_positions_of_occurance(file_data_string,Check_set)
		[closeness_measure,Instruction_distance_list] = compute_closeness_measure(list_of_positions,Block_size)
		
		counter += 1
		avg_closeness_measure += closeness_measure
		if closeness_measure >= max_closeness_measure :
			max_closeness_measure = closeness_measure
			temp = Instruction_distance_list
	closeness_measure = avg_closeness_measure/no_of_tries
	Instruction_distance_list = temp
	#--------------------------Compute closeness of encrypted portions-----------------------------------------------------------#
	suspected_block_list = compute_entropy(file_data_string,Entropy_Block_size)
	[Entropy_closeness_measure,Entropy_block_distance_list] = compute_closeness_measure(suspected_block_list,Entropy_Block_size)
		
	#--------------------------Compute Instruction distance variances------------------------------------------------------------#
	#distance_variance = compute_variance(Instruction_distance_list)
	

	#--------------------------END OF FEATURE VECTOR COMPUTATION-----------------------------------------------------------------
	#feature_vector =[no_of_hits,closeness_measure*closeness_measure,Entropy_closeness_measure]	
	#feature_vector =[harm_factor, no_of_hits, closeness_measure*closeness_measure,Entropy_closeness_measure]
	feature_vector =[harm_factor,closeness_measure*closeness_measure,Entropy_closeness_measure]
	return feature_vector		

def train() :

	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global training_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory
	global weighted_avg_time
	global avg_time
	global total_file_length
	global total_no_of_train_samples
	global total_no_of_packets	

	weighted_avg_time = 0.0
	avg_time = 0.0
	total_file_length = 0.0
	total_no_of_train_samples = 0
	total_no_of_packets = 0
	class_labels = []
	training_set = []

	for root, dirs, files in os.walk(training_files_directory):
		for file in files :
			total_no_of_train_samples += 1
			path = os.path.join(root, file)
			path = path.replace('\\','/')
			with open(path, 'rb') as f:
				content = f.read()
			f.close()
			file_data_string = binascii.hexlify(content)

			#print "Parsing training file : ",total_no_of_train_samples," Name : ", path
			start = 0
			

			#Forget about adding acse header for now. It doesn't make a difference anyway.
			
			total_file_length += float(len(file_data_string))
			

			#-----------------------------------START OF FEATURE VECTOR COMPUTATION------------------------------------------------------#
			# Start measuring time 
			start_time = time.time()
			
			feature_vector = extract_features(file_data_string)
			
			# Measure the time taken to estimate all features
			end_time = time.time()

			weighted_avg_time += float(len(file_data_string))*(end_time - start_time)
			#avg_time += end_time - start_time
			

			#,distance_variance*len(Instruction_distance_list)]
			#feature_vector = [no_of_hits,no_of_hits*closeness_measure*math.log1p(distance_variance),Entropy_closeness_measure]
			#print 'Feature vector ', feature_vector
			for ext in valid_extensions :
				if path.endswith(ext) : 
					class_labels.append(1)
					print feature_vector, " Time spent = ", end_time - start_time
			
			for ext in invalid_extensions :
				if path.endswith(ext) :
					class_labels.append(-1)
					print feature_vector, " Time spent = ", end_time - start_time
					avg_time += end_time - start_time
					total_no_of_packets += 1
			training_set.append(feature_vector)
		
	class_labels = np.array(class_labels)
	training_set = np.array(training_set)		
	print 'Training the SVM now'
	classifier = SVC()
	classifier.fit(training_set,class_labels)

	print 'SVM trained. Now storing on disk'
	
	joblib.dump(classifier,path_to_classifier + "/classifier.pkl")
	
	print 'Trained SVM model has been stored on disk'


def test_for_experiment() :

	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global testing_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory
	global weighted_avg_time
	global avg_time
	global total_file_length
	global total_no_of_test_samples
	global total_no_of_packets

	
	total_no_of_test_samples = 0
	class_labels = []
	testing_set = []
	file_length_list = []
	classifier = joblib.load(path_to_classifier + "/classifier.pkl")

	for root, dirs, files in os.walk(testing_files_directory):
		for file in files :
			total_no_of_test_samples += 1
			path = os.path.join(root, file)
			path = path.replace('\\','/')
			with open(path, 'rb') as f:
				content = f.read()
			f.close()
			file_data_string = binascii.hexlify(content)
			print "Parsing test file : ",total_no_of_test_samples," Name : ", path

			start = 0
			

			#Forget about adding acse header for now. It doesn't make a difference anyway.
			
			total_file_length += float(len(file_data_string))
			

			#-----------------------------------START OF FEATURE VECTOR COMPUTATION------------------------------------------------------#
			# Start measuring time 
			start_time = time.time()
			
			feature_vector = extract_features(file_data_string)

			# Measure the time taken to estimate all features
			end_time = time.time()

			weighted_avg_time += float(len(file_data_string))*(end_time - start_time)
			#avg_time += end_time - start_time
			
			#,distance_variance*len(Instruction_distance_list)]
			#feature_vector = [no_of_hits,no_of_hits*closeness_measure*math.log1p(distance_variance),Entropy_closeness_measure]
			print 'Feature vector ', feature_vector, " Time spent = ", end_time - start_time
			for ext in valid_extensions :
				if path.endswith(ext) : 
					class_labels.append(1)
			
			for ext in invalid_extensions :
				if path.endswith(ext) :
					class_labels.append(-1)
					avg_time += end_time - start_time
					total_no_of_packets += 1
			testing_set.append(feature_vector)
			file_length_list.append(len(file_data_string))
		
	class_labels = np.array(class_labels)
	testing_set = np.array(testing_set)		
	print 'Testing the SVM now'
		
	predicted_labels = classifier.predict(testing_set)
	start = 0
	misclassifications = 0
	false_positives = 0
	false_negatives = 0
	misclassified_files_info = []
	
	while start < total_no_of_test_samples :
		if predicted_labels[start] != class_labels[start] :
			print predicted_labels[start], class_labels[start], "File length : ",file_length_list[start]," Feature vector : ",testing_set[start]
			if class_labels[start] == -1 :
				false_positives += 1
			else :
				false_negatives += 1
			misclassifications += 1
			misclassified_files_info.append([predicted_labels[start], class_labels[start], file_length_list[start],testing_set[start]])
		start += 1
	print 'no of misclassifications ', misclassifications
	print 'total no of test samples ', total_no_of_test_samples
	weighted_avg_time = weighted_avg_time/total_file_length
	#avg_time = avg_time/(float(total_no_of_train_samples + total_no_of_test_samples))
	avg_time = avg_time/(float(total_no_of_packets))
	print "Weighted average of feature extraction time : ", weighted_avg_time
	print "Average per packet feature extraction time : ", avg_time

	if not os.path.exists(output_directory):
		os.makedirs(output_directory)
		
	with open(output_directory + '/output.txt','a') as f :
		f.write("############################################################################################################################" + "\n")
		f.write("No of misclassifications : " + str(misclassifications) + "\n")
		f.write("No of false positive     : " + str(false_positives) + "\n")
		f.write("No of false negatives    : " + str(false_negatives) + "\n")
		f.write("Total no of test samples : " + str(total_no_of_test_samples) + "\n")
		f.write("Weighted Avg of feature extraction time : " + str(weighted_avg_time) + "\n")
		f.write("Avg feature extraction time : " + str(avg_time) + "\n")
		f.write("Accuracy level           : " + str(float(100) - float(misclassifications*100)/float(total_no_of_test_samples)) + "\n\n")
		counter = 0
		while counter < misclassifications :
			f.write("File info : " + str(misclassified_files_info[counter]) + "\n")
			counter += 1
	f.close()
	print "Cleaning up ... "
	shutil.rmtree(training_files_directory) 
	shutil.rmtree(testing_files_directory) 



def predict(byte_string) :
	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global testing_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory
	global weighted_avg_time
	global avg_time
	global total_file_length
	global total_no_of_test_samples

	
	total_no_of_test_samples = 0
	class_labels = []
	testing_set = []
	file_data_string = byte_string

	if path_to_classifier == "" :
		print "Path to classifier undefined. Exiting "
		exit()
	classifier = joblib.load(path_to_classifier + "/classifier.pkl")

	
	
	#Forget about adding acse header for now. It doesn't make a difference anyway.


	feature_vector = extract_features(file_data_string)

	testing_set.append(feature_vector)
	
	testing_set = np.array(testing_set)		
	print "Feature vector : ",feature_vector
	print 'Predicting using the trained classifier now'
		
	predicted_labels = classifier.predict(testing_set)
	
	return predicted_labels[0]	


def plot_features(path_directory) :
	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global testing_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory
	global weighted_avg_time
	global avg_time
	global total_file_length
	global total_no_of_test_samples

	
	total_no_of_test_samples = 0
	class_labels = []
	testing_set = []
	

	harm_factor_list = []
	closeness_measure_list = []
	entropy_closeness_measure_list = []
	PEB_list = []
	CF_hit_list = []
	
	
	for root, dirs, files in os.walk(path_directory):
		for file in files :
			total_no_of_test_samples += 1
			path = os.path.join(root, file)
			path = path.replace('\\','/')
			print "Parsing file ", path
			with open(path, 'rb') as f:
				content = f.read()
			f.close()
			file_data_string = binascii.hexlify(content)
	
	
			#Forget about adding acse header for now. It doesn't make a difference anyway.


			feature_vector = extract_features(file_data_string)
			if len(feature_vector) == 4 :
				harm_factor = feature_vector[0]
				no_of_hits = feature_vector[1]
				closeness_measure = feature_vector[2]
				entropy_closeness_measure =  feature_vector[3]
			elif len(feature_vector) == 3 :
				harm_factor = feature_vector[0]
				no_of_hits = feature_vector[1]
				closeness_measure = feature_vector[2]
				#entropy_closeness_measure =  feature_vector[2]
				

			harm_factor_list.append(harm_factor)
			closeness_measure_list.append(closeness_measure)
			if len(feature_vector) == 4 :
				PEB_list.append(entropy_closeness_measure)
			
			#if len(feature_vector) == 4 :
			CF_hit_list.append(no_of_hits)
				
			
	(harm_factor_bins,harm_factor_freq_list)  = obtain_histogram_plot(harm_factor_list,False)
	(closeness_measure_bins,closeness_measure_freq_list) = obtain_histogram_plot(closeness_measure_list,False)
	(CF_hit_count_bins,CF_hit_count_freq_list) = obtain_histogram_plot(CF_hit_list,True)
	
	
	if len(feature_vector) == 4: 
		(PEB_measure_bins,PEB_measure_freq_list) = obtain_histogram_plot(PEB_list,False)
	
	plot_figure(harm_factor_bins,harm_factor_freq_list,"Harm factor","Frequency in Percentage","Distribution of the harm factor")
	plot_figure(closeness_measure_bins,closeness_measure_freq_list,"Closeness measure","Frequency in Percentage","Distribution of the Closeness measure")
	plot_figure(CF_hit_count_bins,CF_hit_count_freq_list,"Control flow hit counts","Frequency in Percentage","Distribution of the CF hit counts")
	
	if len(feature_vector) == 4 :
		plot_figure(PEB_measure_bins,PEB_measure_freq_list,"PEB measure","Frequency in Percentage","Distribution of the PEB measure")
		#plot_figure(CF_hit_count_bins,CF_hit_count_freq_list,"Control flow hit counts","Frequency in Percentage","Distribution of the CF hit counts")
		pass
		
	

			
def plot_figure(OX,OY,xlabel_str,ylabel_str,title_str) :
	fig = plt.figure()
	width = 1
	ind = np.arange(len(OY))
	plt.bar(ind, OY)
	plt.xticks(ind + width / 2, OX)

	fig.autofmt_xdate()
	plt.xlabel(xlabel_str)
	plt.ylabel(ylabel_str)
	plt.title(title_str)
	plt.show()

	
def obtain_histogram_plot(input_list,is_int) :
	max_value = 0.0
	min_value = 1000000.0
	
		
	no_of_bins = 10
	frequency_list = []
	bins = []
	for entry in input_list :

		if entry < min_value :
			min_value = entry
		elif entry > max_value :
			max_value = entry
	i = 0
	if max_value == min_value :
		return ([min_value],[len(input_list)])
	if is_int == True :
		no_of_bins = max_value - min_value
	while i < no_of_bins :
		frequency_list.append(0)
		if i == 0 :
			bins.append(min_value)
		else :
			bins.append(bins[i-1] + float(max_value - min_value)/float(no_of_bins))
		i += 1
	
	for entry in input_list :
		j = 1
		while j < no_of_bins :
			if entry < bins[j] :
				frequency_list[j - 1] += 1
				break
			j += 1
	
	i = 0
	while i < no_of_bins :
		frequency_list[i] = float(frequency_list[i] * 100)/float(len(input_list))
		bins[i] = round(bins[i],3)
		i += 1
		
		
		
	return (bins,frequency_list)	
	


def calculate_information_gain() :

	global MAX_NO_OF_HITS
	global no_of_instructions_to_check
	global Block_size
	global no_of_rounds
	global Entropy_Block_size
	global Entropy_Block_size_start
	global Entropy_Block_size_end 
	global step_size 
	global Data_transfer_instructions 
	global Control_flow_instructions 
	global no_of_control_flow_instructions 
	global no_of_data_transfer_instructions
	global Valid_Prefix 
	global valid_extensions
	global invalid_extensions
	global training_files_directory
	global output_directory
	global path_to_classifier
	global valid_extensions
	global invalid_extensions
	global dataset_directory
	global weighted_avg_time
	global avg_time
	global total_file_length
	global total_no_of_train_samples
	global total_no_of_packets


	if dataset_directory == "" :
		print "Path to Data set is not set. Cannot Simulate. Exiting"
		exit()
	if output_directory == "" :
		print "Output directory is not set. Cannot Simulate. Exiting"
		exit()

	if path_to_classifier == "" :
		print "Path to classifier is not set. Cannot Simulate. Exiting"
		exit()

	
	
	
	print "Creating training set from the specified Data set"
	if not os.path.exists(training_files_directory):
		os.makedirs(training_files_directory)
	else :
		shutil.rmtree(training_files_directory) 
		print "Training files directory cleaned up"
		os.makedirs(training_files_directory)


	for root, dirs, files in os.walk(dataset_directory):
		for file in files :
			prob = rnd.uniform(0,10)
			path = os.path.join(root, file)
			path = path.replace('\\','/')
			if prob <= 7 : # add it to the training directory
				shutil.copy2(path,training_files_directory)
				
	print "Training set created"	
	print "Computing information gain .. "
	weighted_avg_time = 0.0
	avg_time = 0.0
	total_file_length = 0.0
	total_no_of_train_samples = 0
	total_no_of_packets = 0
	class_labels = []
	training_set = []

	for root, dirs, files in os.walk(training_files_directory):
		for file in files :
			total_no_of_train_samples += 1
			path = os.path.join(root, file)
			path = path.replace('\\','/')
			with open(path, 'rb') as f:
				content = f.read()
			f.close()
			file_data_string = binascii.hexlify(content)
			print "parsing file number : ", total_no_of_train_samples
						
			feature_vector = extract_features(file_data_string)
			
						
			for ext in valid_extensions :
				if path.endswith(ext) : 
					class_labels.append(1)
					
			
			for ext in invalid_extensions :
				if path.endswith(ext) :
					class_labels.append(-1)
					
			training_set.append(feature_vector)
		
	class_labels = np.array(class_labels)
	training_set = np.array(training_set)

	if len(feature_vector) >= 3 :	
		maxGain_HF,bestSplit_HF,bestPart1,bestPart2 = getMaxInfoGain(class_labels,training_set,0)
		maxGain_CM,bestSplit_CM,bestPart1,bestPart2 = getMaxInfoGain(class_labels,training_set,1)
		maxGain_PEB,bestSplit_PEB,bestPart1,bestPart2 = getMaxInfoGain(class_labels,training_set,2)
	if len(feature_vector) == 4 :
		maxGain_NH,bestSplit_NH,bestPart1,bestPart2 = getMaxInfoGain(class_labels,training_set,3)

	#clf = DecisionTreeClassifier(random_state=0)
	#clf.fit(training_set,class_labels)
	#print "Feaure importance = ", clf.feature_importances_()
	svc = SVC(kernel="linear")
	rfecv = RFECV(estimator=svc, step=1)
	rfecv.fit(training_set, class_labels)
	print " rfecv.support = " ,rfecv.support_ , " ranking = ", rfecv.ranking_ ," cv_scores = ", rfecv.cv_scores_
	print("Optimal number of features : %d" % rfecv.n_features_)

	#Plot number of features VS. cross-validation scores
	#plt.figure()
	#plt.xlabel("Iteration number")
	#plt.ylabel("Cross validation score (nb of correct classifications)")
	#plt.plot(range(1, len(rfecv.cv_scores_) + 1), rfecv.cv_scores_)
	#plt.show()
	
	with open(output_directory + '/Inf_gain.txt','a') as f :
		f.write("############################################################################################################################" + "\n")
		if len(feature_vector) >= 3 :
			f.write("MAX Gain HF = " + str(maxGain_HF) + " Best Split = " + str(bestSplit_HF))
			f.write("\n")
			f.write("MAX Gain CM = " + str(maxGain_CM) + " Best Split = " + str(bestSplit_CM))
			f.write("\n")
			f.write("MAX Gain PEB = " + str(maxGain_PEB) + " Best Split = " + str(bestSplit_PEB))
			f.write("\n")
		if len(feature_vector) == 4 :
				f.write("MAX Gain Number of hits = " + str(maxGain_NH) + " Best Split = " + str(bestSplit_NH))
				f.write("\n")
	f.close()
	print "Cleaning up ... "
	shutil.rmtree(training_files_directory) 
	
#------------------------------------------------------------------------------------------------------------------------------------------------------


