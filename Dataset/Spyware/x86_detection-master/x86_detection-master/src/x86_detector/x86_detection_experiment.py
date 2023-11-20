"""
Usage :

<sample_directory> : Usually the dataset which can contain a mixture of x86 executables and c1222 packets. 
<path_to_classifier> : Should contain the stored classifier. The default classifier name is classifier.pkl  (eg : see the classifiers folder)


Running in passive mode as a standalone server process (localhost, port : 2700):
-------------------------------------------------------------------------------

	python x86_detection_experiment <path_to_classifier>

	-Runs as a standalone process which interacts with the c1222 client
	-Should be run in the Client machine
	-c1222 client sends packet bytes to the x86 detector which tests it against a stored classifier and returns the result.

Experiments :
-------------

	python x86_detection_experiment <Test number> <sample_directory*> <path_to_classifier*>

	Test types
	----------

	Test 1 : 

		-Input arguments : <sample_directory> && <path_to_classifier>
		-Iterates through each file in the sample directory and uses the specified classifier to predict whether the file is an x86 executable or a c1222 metering data sample
		-Outputs the number of packets and executables detected.

	Test 2 :

		-Input arguments : <sample_directory> && <path_to_classifier>
		-Uses the specified sample directory as the dataset. Randomly splits it into 70 % training set and 30 % testing set, trains a classifier using the training set and stores the classifier 
		 with the file name classifier.pkl inside the directory specified by <path_to_classifier>
		-The trained classifier is used to predict the class of samples in the test set. An output file 'output.txt' containing the statistics is generated and stored inside <path_to_classifier>
		-The training and test files are destroyed
		

	Test 3 :

		-Input arguments <sample_directory>
		-Iterates through each file in the specified directory and plots histograms of all the features


	Test 4 :

		-Input arguments <path_to_classifier> <byte_sequence> <byte_sequence_length>
		-Uses the classifier specified in the directory to check if the given byte sequence specified as an argument contains x86 content or not.
		
		

"""


from x86_executable_detection import *
import sys
import os
import scipy
import socket

arg_list = sys.argv
if len(arg_list) == 1: 
	print "Not enough arguments. Atleast the path to the classifier must be specified"
	sys.exit(-2)
if len(arg_list) == 2 :
	
	print "Not enough arguments for performing tests. Running in passive mode as a tcp server ..."
	path_to_classifier = arg_list[1]
	path_to_classifier = path_to_classifier.replace('\\','/')
	x86_detector_init()
	set_path_to_classifier(path_to_classifier)
	if os.path.isdir(path_to_classifier) == False :
		print "No such directory ", path_to_classifier," exists"
		sys.exit(-2)
	
	while True :
		HOST = 'localhost'   # Symbolic name meaning all available interfaces
		PORT = 2700 # Arbitrary non-privileged port
 
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		print 'Socket created'
 
		try:
			s.bind((HOST, PORT))
		except socket.error , msg:
			print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
			sys.exit(-2)
     
		print 'Socket bind complete'
 
		s.listen(10)
		print 'Socket now listening'
 
		#wait to accept a connection - blocking call
		conn, addr = s.accept()
 
		print 'Connected with ' + addr[0] + ':' + str(addr[1])
 
		#now keep talking with the client
		file_data_string = conn.recv(4096)
		print "Received string : ", file_data_string
		predicted_label = predict(file_data_string)
		if predicted_label == 1 :
			conn.sendall("It contains x86 executable content")
		else : 
			conn.sendall("It does not contain x86 executable content")
		
		conn.close()
		s.close()
	
	sys.exit(1)
elif len(arg_list) == 3 :
	test = int(arg_list[1])
	if test == 1 or test == 2 or test == 4:
		print "Not enough arguments for test type 1 or 2"
		sys.exit(-2)
	elif test == 3 :
		sample_directory = arg_list[2]
	
elif len(arg_list) == 4 :
	test = int(arg_list[1])
	if test == 1 or test == 2:
		sample_directory = arg_list[2]
		path_to_classifier = arg_list[3]
		output_directory = path_to_classifier
	elif test == 3 :
		print "Incorrect number of arguments for test type 3"
		sys.exit(-2)
	else :
		print "Incorrect number of arguments for test type 4"
		sys.exit(-2)
elif len(arg_list) == 5 :
	test = int(arg_list[1])
	if test == 4 :
		path_to_classifier = arg_list[2]
		path_to_classifier = path_to_classifier.replace('\\','/')
		data_string = arg_list[3]
		data_length = int(arg_list[4])
	else :
		print "Incorrect number of arguments"
		sys.exit(-2)
else :
	print "Incorrect number of arguments"
	sys.exit(-2)

if test == 1 or test == 2:
		if os.path.isdir(sample_directory) == False or os.path.isdir(output_directory) == False or os.path.isdir(path_to_classifier) == False:
			print "Directories passed as arguments are invalid"
			sys.exit(-2)
elif test == 3 :
		if os.path.isdir(sample_directory) == False :
			print "No such directory ", sample_directory, " exists"
			sys.exit(-2)
elif test == 4: 
		if os.path.isdir(path_to_classifier) == False :
			print "No such directory ", path_to_classifier," exists"
			sys.exit(-2)


if test == 1 :
	x86_detector_init()
	init_experiment(sample_directory,output_directory,path_to_classifier)
	no_of_executables_detected = 0
	no_of_packets_detected = 0	
	set_path_to_classifier(path_to_classifier)
	for root, dirs, files in os.walk(sample_directory):
		for file in files :
			total_no_of_test_samples += 1
			path = os.path.join(root, file)
			path = path.replace('\\','/')
			print "Parsing file ", path
			with open(path, 'rb') as f:
				content = f.read()
			f.close()
			file_data_string = binascii.hexlify(content)

			predicted_label = predict(file_data_string)
	
			if predicted_label == 1 :
				print "It contains executable content"
				no_of_executables_detected += 1
			else :
				print "It does not contain executable content"
				no_of_packets_detected += 1

	print "No of executables detected = ",no_of_executables_detected
	print "No of packets detected = ",no_of_packets_detected
	sys.exit(1)
elif test == 2 :
	x86_detector_init()
	dataset_directory = sample_directory
	init_experiment(dataset_directory,output_directory,path_to_classifier)
	no_of_executables_detected = 0
	no_of_packets_detected = 0
	simulate_experiment(no_of_iterations = 1)
	#calculate_information_gain()
	sys.exit(1)
	pass
elif test == 3 :
	x86_detector_init()
	plot_features(sample_directory)
	sys.exit(1)
elif test == 4 :
	x86_detector_init()
	
	set_path_to_classifier(path_to_classifier)
	file_data_string = data_string[0:data_length]
	#file_data_string = binascii.hexlify(file_data_string)

	print "Received file data string " , file_data_string
	print "Received length  : ", len(file_data_string), " Expected length : ", data_length
	predicted_label = predict(file_data_string)
	if predicted_label == 1 :
		sys.exit(1)
	else : 
		sys.exit(0)
else :	
	print "Incorrect test type"
	sys.exit(-2)
	
