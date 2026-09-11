
:meta-keywords: do statement
:meta-description: The DO statement executes the specified expression, but does not return the result.

**
DO
**

**DO** 문은 지정된 연산식을 실행하지만 결과 값을 리턴하지 않는다. 지정된 연산식이 문법에 맞게 쓰여지지 않으면 에러를 반환하므로, 연산식의 문법이 올바른지 여부를 확인하는 데 사용할 수 있다. **DO** 문은 데이터베이스 서버에서 연산 결과 또는 에러를 반환하지 않기 때문에, 일반적으로 **SELECT** 문보다 수행 속도가 빠르다. ::

    DO expression

*   *expression* : 임의의 연산식을 지정한다.

.. code-block:: sql

    DO 1+1;
    DO SYSDATE + 1;
    DO (SELECT count(*) FROM athlete);
