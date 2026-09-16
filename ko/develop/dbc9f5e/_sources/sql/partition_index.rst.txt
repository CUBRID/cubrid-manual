
:meta-keywords: cubrid partition, partitioning key, range partition, hash partition, list partition, partition pruning
:meta-description: Partitioning is a method by which a table is divided into multiple independent physical units called partitions. In CUBRID, each partition is a table implemented as a subclass of the partitioned table.

****
분할
****

분할 기법(partitioning)은 하나의 테이블을 분할(partition)이라는 여러 독립적인 물리적 단위로 나눠주는 기법이다. CUBRID에서 각 분할은 분할 테이블(partitioned table)의 서브클래스로 구현된 테이블이다. 각 분할은 :ref:`partitioning-key`\ 와 분할 방식에 의해 분할 테이블 데이터의 일부분을 보유한다. 사용자는 분할 테이블에 질의문을 수행하여 분할된 데이터에 접근할 수 있다. 이것은 사용자가 해당 테이블에 접근하는데 사용되는 질의문이나 코드를 변경하지 않고 테이블을 분할할 수 있다는 것을 의미한다. 즉 응용 프로그램을 거의 변경하지 않고 분할의 이점을 얻을 수 있게 해준다.

분할 기법은 관리 편의성, 성능 및 가용성을 향상시킬 수 있다. 테이블을 분할하면 다음과 같은 이점이 있다.

*   대용량 테이블의 관리 편의성 향상
*   데이터 조회 시 접근 범위를 줄임으로써 성능 향상
*   디스크 I/O를 분산함으로써 성능 향상 및 물리적 부하 감소
*   여러 분할로 나눔으로써 전체 데이터의 훼손 가능성 감소 및 가용성 향상
*   스토리지 비용의 최적화

분할된 데이터는 시스템에 의해 자동으로 관리된다. 분할 테이블에서 실행되는 :doc:`INSERT<query/insert>` 문과 :doc:`UPDATE<query/update>` 문은 실행하는 동안 레코드가 어느 분할에 위치해야 하는지 확인하기 위한 과정을 수행하게 된다. 시스템은 UPDATE 문이 수행되는 동안 수정된 레코드가 어떤 분할로 이동해야 할지를 확인하고 이동시켜줌으로써 분할 정의를 일관되게 유지한다. 유효하지 않은 분할에 레코드를 삽입하려고 하면 오류를 반환한다.

:doc:`SELECT<query/select>` 문을 수행할 때 시스템은 검색 조건에 해당하는 결과를 보유하고 있는 분할만을 대상으로 하여 검색 공간을 좁히기 위해 :ref:`partition-pruning` 작업을 적용한다. SELECT 문 수행 중에 대부분의 분할을 프루닝(제거)하는 작업은 성능을 크게 향상시킨다.

테이블 분할 기법은 큰 테이블에 적용할 때 가장 효과적이다. "큰" 테이블의 정확한 의미는 응용 프로그램과 테이블이 질의문에서 사용되는 방법에 달려있다. 테이블에 대해 :ref:`영역 <range-partitioning>`, :ref:`리스트 <list-partitioning>` 또는 :ref:`해시 <hash-partitioning>` 중 어느 것이 최적의 분할 방법인지는 테이블이 질의문에서 어떻게 사용되며 데이터가 어떻게 분할되는가에 따라 달라진다. 분할 테이블을 마치 일반 테이블처럼 사용할 수도 있지만, 분할 테이블을 사용할 때는 :ref:`partitioning-notes`\ 을 고려해야 한다.


.. toctree::
    :maxdepth: 2

    partition.rst

