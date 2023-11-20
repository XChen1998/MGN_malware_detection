# Activity #3: Backdoor  

* [./interpreter/](./interpreter): Mini Language Interpreter  
  * INPUT/OUTPUT
    * INPUT: Mini Language bytecodes file  
    * OUTPUT: 입력으로 주어진 bytecodes의 실행 결과 (주어진 Mini Language는 stdout만을 지원)  
  
  * [interpreter.c](./interpreter/interpreter.c): 인터프리터 코드(14개의 opcode와 error handling 구현)  
  * [minivm.c](./interpreter/minivm.c): 에러 메시지를 출력해주는 함수 및 VM의 기본적인 요소(initVMContext, stepVMContext, dispatch) 코드  
  * [minivm.h](./interpreter/minivm.h): minivm.c의 헤더 파일  

* [./login/login.mini](./login/login.mini): login pseudocode in C와 동일하게 동작하는 Mini Language 코드  

* [./test/](./test): 입력하는 문자열을 삽입정렬해주는 Mini Language 프로그램과 간단한 설명  
  * [test.mini](./test/test.mini): Mini Language로 작성된 프로그램(입력하는 문자열을 삽입정렬 후 출력)  
  * [test.md](./test/test.md): test.mini에 대한 설명(pseudocode in C 포함)  

* [./backdoor/](./backdoor): Mini Language Interpreter with backdoor  
  * INPUT/OUTPUT: 위의 interpreter와 동일  

  * [interpreter.c](./backdoor/interpreter.c): (backdoor가 포함되어 있는) 인터프리터 코드 - (./login/login.mini가 입력으로 주어질 경우에만 backdoor 동작)  
  * [minivm.c](./backdoor/minivm.c): ./interpreter/minivm.c 와 동일  
  * [minivm.h](./backdoor/minivm.h): ./interpreter/minivm.h 와 동일  

* Activity #3을 하면서 배운 점  
사실 과제 이해하기도 벅차서 주어진 강의자료와 Ken Thompson자료를 몇번씩 읽은 후에야 이해하게 되었습니다.  
과제를 완성하는데에도 오랜 시간이 소요되었으나 덕분에 compiler, interpreter에 대한 지식을 넓힐 수 있었으며, 프로그램을 구현하면서 어떻게 하면 버그를 줄일 수 있을까라는 생각을 많이 하게 되었습니다.  
물론, 버그를 줄일 수 있을까라는 생각에 대한 해답은 얻지 못했지만, 고민한 것만으로 의미가 있었다고 생각합니다.
