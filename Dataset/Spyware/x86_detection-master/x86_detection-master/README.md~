### X86 DETECTION


### 1.0 Folder Descriptions :

* src/ : Contains all the source files

* classifier/ : Holds the trained classifier

* src/c1222/stack :  Contains all files required to run a c1222 client integrated with the policy engine and a c1222 server

* src/x86_detector : Contains all the source files of the x86 detector


### 2.0 Compilation Instructions :


### 2.1 System Requirements :


	Windows XP or higher, bcc32 compiler v2.6 or higher


### 2.2 Compiling the Client :


	$$ cd x86_detection/src/c1222/stack/client

	$$ make

	$$ cd OUT

	$$ make


### 2.3 Compiling the Server :


	$$ cd x86_detection/src/c1222/stack/server

	$$ make

	$$ cd OUT

	$$ make

### 2.4 Starting the Server :


	$$ cd x86_detection/src/c1222/stack/server/OUT/Bin/

	$$ ./c1222test_server.exe


### 2.5 Starting the x86 detector : 

Start the x86 detector before the Client


	$$ cd x86_detection/src/x86_detector

	$$ python x86_detection_experiment <path_to_classifier>

	e.g:

	$$ python x86_detector x86_detection_experiment x86_detection/classifier


### 2.6 Starting the Client :


	$$ cd x86_detection/src/c1222/stack/client/OUT/Bin/

	$$ ./c1222test_client.exe

