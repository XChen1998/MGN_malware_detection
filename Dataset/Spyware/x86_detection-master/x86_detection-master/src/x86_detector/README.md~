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
		
		
