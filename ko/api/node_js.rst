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

CUBRRID Node.js 드라이버를 다운로드하거나 CUBRID Node.js 드라이버에 대한 최신 정보를 확인하려면 `http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver>`_ 에 접속한다.


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

Node.js 예제 프로그램
=====================

CUBRID Node.js에 대한 보다 많은 예제 코드는 `https://github.com/CUBRID/node-cubrid <https://github.com/CUBRID/node-cubrid>`_ 에 있는 demo 폴더를 참고한다.

다음은 드라이버 이벤트 모델을 사용하는 표준 코딩 예제이다.  ::

	CUBRIDClient.connect();

	CUBRIDClient.on(CUBRIDClient.EVENT_ERROR, function (err) {
	  Helpers.logError('Error!: ' + err.message);
	});

	CUBRIDClient.on(CUBRIDClient.EVENT_CONNECTED, function () {
	  Helpers.logInfo('Connected.');
	  Helpers.logInfo('Querying: select * from game');
	  CUBRIDClient.query('select * from game', function () {
	  });
	});

	CUBRIDClient.on(CUBRIDClient.EVENT_QUERY_DATA_AVAILABLE, function (result, queryHandle) {
	  Helpers.logInfo('Data received.');
	  Helpers.logInfo('Returned active query handle: ' + queryHandle);
	  Helpers.logInfo('Total query result rows count: ' + Result2Array.TotalRowsCount(result));
	  Helpers.logInfo('First "batch" of data returned rows count: ' + Result2Array.RowsArray(result).length);
	  Helpers.logInfo('Fetching more rows...');
	  CUBRIDClient.fetch(queryHandle, function () {
	  });
	});

	CUBRIDClient.on(CUBRIDClient.EVENT_FETCH_DATA_AVAILABLE, function (result, queryHandle) {
	  Helpers.logInfo('*** Fetch data received for query: ' + queryHandle);
	  Helpers.logInfo('*** Current fetch of data returned rows count: ' + Result2Array.RowsArray(result).length);
	  Helpers.logInfo('*** First row: ' + Result2Array.RowsArray(result)[0].toString());
	  // continue to fetch...
	  Helpers.logInfo('...');
	  Helpers.logInfo('...fetching more rows...');
	  Helpers.logInfo('...');
	  CUBRIDClient.fetch(queryHandle, function () {
	  });
	});

	CUBRIDClient.on(CUBRIDClient.EVENT_FETCH_NO_MORE_DATA_AVAILABLE, function (queryHandle) {
	  Helpers.logInfo('No more data to fetch.');
	  Helpers.logInfo('Closing query: ' + queryHandle);
	  CUBRIDClient.closeQuery(queryHandle, function () {
	  });
	});

	CUBRIDClient.on(CUBRIDClient.EVENT_QUERY_CLOSED, function (queryHandle) {
	  Helpers.logInfo('Query closed: ' + queryHandle);
	  Helpers.logInfo('Closing connection...');

	  CUBRIDClient.close(function () {
	  });
	});

	CUBRIDClient.on(CUBRIDClient.EVENT_CONNECTION_CLOSED, function () {
	  Helpers.logInfo('Connection closed.');
	});

다음은 잘 알려진 Async.js 라이브러리(https://github.com/caolan/async)를 사용하는 예제이다.  ::
	
	ActionQueue.enqueue(
	[
	    function (cb) {
	        CUBRIDClient.connect(cb);
	    },
	    
	    function (cb) {
	        CUBRIDClient.getEngineVersion(cb);
	    },
	    
	    function (engineVersion, cb) {
	        Helpers.logInfo('Engine version is: ' + engineVersion);
	        CUBRIDClient.query('select * from code', cb);
	    },
	    
	    function (result, queryHandle, cb) {
	        Helpers.logInfo('Query result rows count: ' + Result2Array.TotalRowsCount(result));
	        Helpers.logInfo('Query results:');
	        var arr = Result2Array.RowsArray(result);
	        for (var k = 0; k < arr.length; k++) {
	            Helpers.logInfo(arr[k].toString());
	        }
	        CUBRIDClient.closeQuery(queryHandle, cb);
	        Helpers.logInfo('Query closed.');
	    },
	    
	    function (cb) {
	        CUBRIDClient.close(cb);
	        Helpers.logInfo('Connection closed.');
	    }
	],
	
	function (err) {
	    if (err == null) {
	        Helpers.logInfo('Program closed.');
	    } else {
	        throw err.message;
	    }
	}
	);
	
표준 콜백 스타일을 선호한다면 다음과 같이 작성할 수 있다. ::
	
	CUBRIDClient.connect(function (err) {
	  if (err) {
	    errorHandler(err);
	  } else {
	    Helpers.logInfo('Connected.');
	    Helpers.logInfo('Querying: select * from nation');
	    CUBRIDClient.query('select * from nation', function (err, result, queryHandle) {
	      if (err) {
	        errorHandler(err);
	      } else {
	        assert(Result2Array.TotalRowsCount(result) === 215);
	        Helpers.logInfo('Query result rows count: ' + Result2Array.TotalRowsCount(result));
	        var arr = Result2Array.RowsArray(result);
	        for (var j = 0; j < 1; j++) {
	          Helpers.logInfo(arr[j].toString());
	        }
	        CUBRIDClient.closeQuery(queryHandle, function (err) {
	          if (err) {
	            errorHandler(err);
	          } else {
	            Helpers.logInfo('Query closed.');
	            CUBRIDClient.close(function (err) {
	              if (err) {
	                errorHandler(err);
	              } else {
	                Helpers.logInfo('Connection closed.');
	                Helpers.logInfo('Test passed.');
	              }
	            })
	          }
	        })
	      }
	    })
	  }
	});

Node.js Classes
===============

Node.js 클래스에 대한 자세한 내용은 `CUBRID Node.js 클래스 <http://www.cubrid.org/manual/api/node.js/1.1/index.html>`_ 를 참고한다.

* `_global_ <http://www.cubrid.org/manual/api/node.js/1.1/symbols/_global_.html>`_
* `BatchExecuteNoQueryPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/BatchExecuteNoQueryPacket.html>`_
* `ClientInfoExchangePacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/ClientInfoExchangePacket.html>`_
* `CloseDatabasePacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/CloseDatabasePacket.html>`_
* `CloseQueryPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/CloseQueryPacket.html>`_
* `ColumnMetaData <http://www.cubrid.org/manual/api/node.js/1.1/symbols/ColumnMetaData.html>`_
* `CommitPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/CommitPacket.html>`_
* `CUBRIDConnection <http://www.cubrid.org/manual/api/node.js/1.1/symbols/CUBRIDConnection.html>`_
* `ExecuteQueryPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/ExecuteQueryPacket.html>`_
* `exports.ColumnNamesArray <http://www.cubrid.org/manual/api/node.js/1.1/symbols/exports.ColumnNamesArray.html>`_
* `exports.ColumnTypesArray <http://www.cubrid.org/manual/api/node.js/1.1/symbols/exports.ColumnTypesArray.html>`_
* `exports.RowsArray <http://www.cubrid.org/manual/api/node.js/1.1/symbols/exports.RowsArray.html>`_
* `exports.TotalRowsCount <http://www.cubrid.org/manual/api/node.js/1.1/symbols/exports.TotalRowsCount.html>`_
* `FetchPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/FetchPacket.html>`_
* `GetEngineVersionPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/GetEngineVersionPacket.html>`_
* `GetSchemaPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/GetSchemaPacket.html>`_
* `LOBReadPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/LOBReadPacket.html>`_
* `Number <http://www.cubrid.org/manual/api/node.js/1.1/symbols/Number.html>`_
* `OpenDatabasePacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/OpenDatabasePacket.html>`_
* `PacketReader <http://www.cubrid.org/manual/api/node.js/1.1/symbols/PacketReader.html>`_
* `PacketWriter <http://www.cubrid.org/manual/api/node.js/1.1/symbols/PacketWriter.html>`_
* `ResultInfo <http://www.cubrid.org/manual/api/node.js/1.1/symbols/ResultInfo.html>`_
* `RollbackPacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/RollbackPacket.html>`_
* `SetAutoCommitModePacket <http://www.cubrid.org/manual/api/node.js/1.1/symbols/SetAutoCommitModePacket.html>`_
* `String <http://www.cubrid.org/manual/api/node.js/1.1/symbols/String.html>`_