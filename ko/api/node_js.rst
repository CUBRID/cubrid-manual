
:meta-keywords: cubrid node.js driver, cubrid node.js api, cubrid node.js programming
:meta-description: CUBRID Node.js driver is developed in 100% JavaScript and does not require specific platform compilation.

****************
Node.js 드라이버
****************

CUBRRID Node.js 드라이버는 100% 순수 자바스크립트로 개발되었으며, 특정 플랫폼 상에서의 컴파일을 필요로 하지 않는다.

Node.js는 `크롬의 자바스크립트 런타임 <https://en.wikipedia.org/wiki/V8_(JavaScript_engine)>`_ 상에서 빌드된 플랫폼이다.

Node.js는 다음의 특징을 가지고 있다.

* 이벤트에 의해 제어되는(event-driven) 서버 측 자바스크립트이다.
* 동시에 여러 종류의 많은 I/O를 다루는데 적합하다.
* 블로킹 없는(non-blocking) I/O 모델이라 경량이며 효율적이다.

보다 자세한 사항은 https://nodejs.org/ko/ 를 참고한다.

큐브리드 Node.js 드라이버를 다운로그하거나 최신의 정보를 찾고자 할 때는 아래의 사이트를 참고한다. 

.. FIXME: *   Introducing project: http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver

*   소스코드 메인 저장소: https://github.com/CUBRID/node-cubrid

Node.js 설치
============

**기본 환경**

*   CUBRID 8.4.1 Patch 2 이상
*   `Node.js <https://nodejs.org/ko/>`_

**설치**

CUBRID Node.js 드라이버는 먼저 https://nodejs.org/ko/download/ 에서 node.js를 설치한 후, npm(Node Packaged Modules) install 명령을 사용하여 설치할 수 있다. ::

    npm install node-cubrid

언인스톨하려면 다음 명령을 수행한다. ::

    npm uninstall node-cubrid

CUBRID Node.js API
==================

http://ftp.cubrid.org/CUBRID_Docs/Drivers/Node.js/\를 참고한다.
