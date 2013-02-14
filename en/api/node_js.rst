**************
Node.js Driver
**************

CUBRID Node.js driver is developed in 100% JavaScript and does not require specific platform compilation

Node.js is a platform built on `Chrome's JavaScript runtime <http://code.google.com/p/v8/>`_.

Node.js has the following specifics.

* Event-driven, server-side JavaScript.
* Good at handling lots of different kinds of I/O at the same time.
* Non-blocking I/O model that makes it lightweight and efficient

For more details, see `http://nodejs.org/ <http://nodejs.org/>`_.

If you want to download CUBRRID Node.js driver or find the recent information of CUBRID Node.js driver, see `http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-node-js-driver>`_ .


Installing Node.js 
==================

**Requirements**

*   CUBRID 8.4.1 Patch 2 이상
*   `Node.js <http://nodejs.org/>`_

**Installation**

You can install CUBRID Node.js driver with "npm(Node Packaged Modules) install" command, but firstly you need to install node.js on `http://nodejs.org/download/ <http://nodejs.org/download/>`_. ::

	npm install node-cubrid

If you uninstall CUBRID Node.js driver, do the following command. ::

	npm uninstall node-cubrid

Node.js Sample Program
======================

For more example codes for CUBRID Node.js, see demo folder on `https://github.com/CUBRID/node-cubrid <https://github.com/CUBRID/node-cubrid>`_.

Here is a stadard coding example, using the driver events model:

::

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

Here is another driver usage example, using the well-known async library (https://github.com/caolan/async):

::
	
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
	
Or, if you prefer the standard callbacks "style":

::
	
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

For more information of Node.js classes, see `CUBRID Node.js Classes <http://www.cubrid.org/manual/api/node.js/1.1/index.html>`_.

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