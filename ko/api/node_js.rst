****************
Node.js 드라이버
****************

CUBRRID Node.js 드라이버는 100% 순수 자바스크립트로 개발되었으며, 특정 플랫폼 상에서의 컴파일을 필요로 하지 않는다.

Node.js는 `크롬의 자바스크립트 런타임 <http://code.google.com/p/v8/>`_ 상에서 빌드된 플랫폼이다.

Node.js는 다음의 특징을 가지고 있다.

* 이벤트에 의해 제어되는(event-driven) 서버 측 자바스크립트이다.
* 동시에 여러 종류의 많은 I/O를 다루는데 적합하다.
* 블로킹 없는(non-blocking) I/O 모델이라 경량이며 효율적이다.

보다 자세한 사항은 `http://nodejs.org/ <http://nodejs.org/>`_ 를 참고한다.

CUBRRID Node.js 드라이버를 다운로드하거나 CUBRID Node.js 드라이버에 대한 최신 정보는 아래 사이트에서 확인할 수 있다.
 
* 프로젝트 소개: `http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver>`_
* 소스코드 메인 저장소: `https://github.com/CUBRID/node-cubrid <https://github.com/CUBRID/node-cubrid>`_
 
Node.js 설치
============

**기본 환경**

*   CUBRID 8.4.1 Patch 2 이상
*   `Node.js <http://nodejs.org/>`_

**설치**

CUBRID Node.js 드라이버는 먼저 http://nodejs.org/download/에서 node.js를 설치한 후, npm(Node Packaged Modules) install 명령을 사용하여 설치할 수 있다. ::

	npm install node-cubrid

언인스톨하려면 다음 명령을 수행한다. ::

	npm uninstall node-cubrid

CUBRID Node.js 프로그래밍
=========================

연결
----
* `Node.js 드라이버로 CUBRID에 연결하기 <http://www.cubrid.org/wiki_apis/entry/connecting-to-cubrid-through-node-js-driver>`_

예제
----
* `CUBRID Node.js API를 사용하는 일반적인 예제 <http://www.cubrid.org/blog/cubrid-appstools/common-uses-of-cubrid-nodejs-api-with-examples/>`_
* `CUBRID Node.js Driver를 사용하여 질의 수행하기 <http://www.cubrid.org/wiki_apis/entry/executing-queries-with-cubrid-node-js-driver>`_
* `이벤트를 사용하는 예제 <https://github.com/CUBRID/node-cubrid#usage>`_
* `비동기 Node.js 모듈 기반의 예제 <https://github.com/CUBRID/node-cubrid#usage>`_
* `간단한 콜백 스타일 예제 <https://github.com/CUBRID/node-cubrid#usage>`_
* CUBRID Node.js 드라이버 데모 동영상(`Vimeo <https://vimeo.com/cubrid/nodejs-driver>`_, `Youtube <http://www.youtube.com/watch?v=9V4pW-BcyFU&list=UUn7F3CqcEgadeiw-QSWtMjQ&index=1&feature=plcp>`_)

CUBRID Node.js API
==================
* `CUBRID Node.js 1.1 문서 <http://www.cubrid.org/manual/api/node.js/1.1/index.html>`_
* `CUBRID Node.js 1.0 문서 <http://www.cubrid.org/manual/api/node.js/1.0/index.html>`_
