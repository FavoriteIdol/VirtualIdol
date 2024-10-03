# 가상 공연 플랫폼

Developed with Unreal Engine 5
---

**일정 : 2024.09.30 ~ 2024. 11.30**
---
<br/> 

**마일스톤으로 큰단위의 작업을 설정해 주세요**

<br/> 

**마일스톤 컨벤션**
- 프로토타입, 알파, 베타 순으로 개인으로 마일스톤 작성

  
**마일스톤 컨벤션 예시**
- Title : HSW_prototype

<br/> 

**이슈 작성 후 작업 진행 바랍니다.**

<br/> 

**이슈 예**
**[Feat] 블록골램 걷기 구현**

### ISSUE

- Type: feat
- Detail: c++ 블록골램에 모션 기능 추가

### TODO

- [ ]  팔 모션 주기
- [ ]  다리 모션 주기
- [ ]  실제 물리 공간 이동

<br/> 

**작은 변경사항이라도 꼭 중간마다 커밋, 푸쉬 부탁드립니다.**

<br/> 

**push 전에는 서로 공유 부탁드립니다.**
<br/> 

**브렌치명 컨벤션**
- 브렌치 명 : <자기이니셜>_<기능>

**브렌치명 컨벤션 예**

- HSW_PlayerMove
- CJH_BasicSetting

<br/> 

**커밋 컨벤션**

- [유형] <내용> #이슈번호
- Add : 파일이 추가된 경우
- Feat : 새로운 기능을 추가할 경우
- Edit : 코드나 에디터 등 수정이 이루어진 경우
- Fix : 버그를 고친 경우
- Rename : 파일 혹은 폴더명을 수정하거나 경로 수정을 한 경우
- Remove : 파일을 삭제하는 경우
- !HOTFIX : 치명적인 버그를 고친 경우
- Style : Norm 규정, 세미콜론 누락, 코드수정이 없는 경우
- Refactor : 리팩토링
- Comment : 주석 추가 및 변경
- Docs : 문서를 수정한 경우
- Test : 테스트 추가


**커밋 컨벤션 예**

- [Feat] tokenizer 구현
- [Fix] tokenizer 문자열 파싱 버그 수정
- [Remove] test.txt 임시파일 삭제
- [Rename] test.txt → operator.txt

<br/> 

**코드 컨벤션**

- 블루프린트 이름 규칙: BP_블루프린트이름_이니셜
- 클래스 이름 규칙: 클래스이름_이니셜
- 전역변수: 파스칼 표기법 
- 지역변수: 카멜 표기법 
- 불 변수: b로 시작
  

** 코드 컨벤션 예**

블루프린트, 클래스 이름
- BP_TestCharacter_HSW
- NetGameInstance_CJH
- Player_KMK

변수 이름
- 전역변수: ShootingPlayer
- 지역변수: gameInstance
- 불변수 : bIsPlaying

  
