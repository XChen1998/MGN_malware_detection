# Backdoor with mini-vm

## /interpreter/minivm.h
 - VMContext 구조체에 instruction index와 heap memory ptr 변수 추가

## /interpreter/minivm.c
 - initVMContext 함수에서 구조체에 새로 정의된 변수들 초기값 설정
 - stepVMContext 함수에서 instruction을 읽기 위해 pc와 instruction idex를 함께 사용하는 형태로 변경(jump instruction 함수 구현하기 위해)

## /interpreter/interpreter.c
 - 14개의 instruction 함수들 생성
 - pc 포인터를 이용하여 입력 파일로부터 4바이트씩 읽어 단계적으로 instruction 실행
 - interpreter의 실행원리에 대해 간략하게나마 알 수 있었다.

## /login/login.mini
 - high-level language 보다 낮은 레벨에서 시스템이 돌아가는 것을 생각하게 되었다.
 
## /test/
 - 한정된 instuction으로도 다양한 프로그램 생성이 가능하였다.
 
## /backdoor/
 - 입력값에 'superuser'를 입력하면 eq instruction의 결과가 항상 1이 되도록 변경
 - user명에 'superuser'입력 시, 임의 비밀번호로 superuser 로그인 가능
 - password에 'superuser' 입력 시, user의 비밀번호 몰라도 user 로그인 가능
 - 어플리케이션이 아닌 interpreter에서도 백도어가 동작할 수 있다는 것을 깨달았으며, 시스템이 secure 하다는 것은 확신하기 어려우며 모든 요소들에 대한 trust를 고려해야겠다.
 
