-----------------------------
트랜잭션 커밋과 롤백 지원
-----------------------------

저장 프로시저와 저장 함수에서는 트랜잭션을 **COMMIT**\하거나 **ROLLBACK**\하여 작업을 완료하거나 취소할 수 있다. 이는 데이터 무결성을 유지하고, 트랜잭션 처리의 유연성을 제공하는 중요한 기능이다.
저장 프로시저와 함수 내부의 로직에서 조건이나 예외 처리에 따라 트랜잭션을 제어할 수 있어 복잡한 데이터 로직을 안전하고 효율적으로 관리하는데 큰 도움을 줄 수 있다.

조건 및 예외 처리와 트랜잭션 제어의 장점
---------------------------------------------

1. **조건 기반 트랜잭션 처리**  
   저장 프로시저와 함수 내부에서 조건에 따라 트랜잭션을 커밋하거나 롤백할 수 있다. 예를 들어, 특정 조건이 만족되지 않을 경우 롤백하여 데이터의 무결성을 보장할 수 있다.

2. **예외 발생 시 안전한 롤백**  
   저장 프로시저 내부에서 오류(예외)가 발생하면 해당 트랜잭션을 롤백하고, 작업을 안전한 상태로 되돌릴 수 있다. 이는 예기치 않은 상황에서도 데이터가 손상되지 않도록 하는 데 유용하다.

3. **복잡한 로직의 처리 안정성**  
   여러 작업이 포함된 복잡한 로직을 처리하면서도, 상황에 따라 트랜잭션의 성공 여부를 명확히 제어할 수 있다. 이를 통해 데이터 처리의 신뢰성을 높일 수 있다.


예제: 조건 및 예외 처리와 트랜잭션 제어
------------------------------------------

다음은 저장 프로시저에서 조건과 예외 처리를 활용하여 트랜잭션을 제어하는 예제이다

.. code-block:: sql

        ;server-output on

        CREATE TABLE orders (
          id INT NOT NULL PRIMARY KEY,
          product VARCHAR(100) NOT NULL,
          quantity INT NOT NULL
        );

        CREATE TABLE inventory (
          product VARCHAR(100) NOT NULL PRIMARY KEY,
          stock INT NOT NULL
        );

        CREATE OR REPLACE PROCEDURE process_transaction() 
        AS
        current_stock INT;
        BEGIN
                INSERT INTO orders (id, product, quantity) VALUES (1, 'Laptop', 5);

                UPDATE inventory SET stock = stock - 5 WHERE product = 'Laptop';

                SELECT stock INTO current_stock FROM inventory WHERE product = 'Laptop';

                IF current_stock < 0 THEN
                  ROLLBACK;
                  DBMS_OUTPUT.PUT_LINE('Transaction failed: Insufficient stock.');
                ELSE
                  COMMIT;
                  DBMS_OUTPUT.PUT_LINE('Transaction successful.');
                END IF;
        EXCEPTION
                WHEN OTHERS THEN
                  ROLLBACK;
                  DBMS_OUTPUT.PUT_LINE('Transaction failed: An error occurred.');
        END;

::

        process_transaction ();

        === <Result of CALL Command in Line 2> ===

        Result              
        ======================
        NULL                

        <DBMS_OUTPUT>
        ====
        Transaction successful.

이 예제는 다음과 같은 시나리오를 다룬다:
- 재고가 부족한 경우 트랜잭션을 롤백하여 데이터의 무결성을 유지한다.
- 작업 중 예외가 발생하면 트랜잭션을 안전하게 롤백한다.
- 조건이 만족되면 트랜잭션을 커밋하여 작업을 완료한다.

주의사항
----------

* PL/CSQL에서는 기본적으로 **COMMIT**과 **ROLLBACK** 문을 지원하며, 이를 통해 저장 프로시저 내부에서 트랜잭션 처리를 명시적으로 제어할 수 있다
* 그러나 Java 언어의 경우, 하위호환성을 유지하기 위해 **pl_transaction_control** 설정을 활용해야 하며 **yes**로 설정하는 경우에만 COMMIT과 ROLLBACK 문을 사용할 수 있다. 
* **pl_transaction_control**의 기본값은 **no**로 설정되어 있으며 COMMIT과 ROLLBACK이 무시된다.
