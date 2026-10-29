***********
다국어 지원
***********

다국어 지원(Globalization)은 다양한 언어와 지역에 적용될 수 있도록 하는 국제화(Internationalization)와 언어별 구성 요소를 추가하여 특정 지역의 언어나 문화에 맞추는 현지화(Localization)를 포함한다. CUBRID는 현지화를 쉽게 하기 위해 유럽과 아시아를 포함한 여러 언어들의 콜레이션(collation)을 지원한다.

문자 데이터 설정에 대한 전반적인 사항을 알고 싶다면 :ref:`char-data-conf-guide`\ 를 참고한다.

문자셋, 콜레이션, 로캘이 무엇인지 알고 싶다면 :ref:`globalization-overview`\ 를 참고한다.

타임존 타입 및 관련 함수들은 :ref:`timezone-type`\을 참고한다. 타임존 관련 시스템 파라미터는 :ref:`timezone-parameters`\를 참고한다. 타임존 정보를 새로운 정보로 업데이트하고 싶은 경우 타임존 라이브러리를 재컴파일해야 되며, 이와 관련하여 :ref:`timezone-library`\를 참고한다.

원하는 언어와 지역에 따른 로캘을 데이터베이스에 반영하고 싶으면 반드시 로캘을 먼저 설정한 후 데이터베이스를 생성해야만 한다. 이와 관련하여 :ref:`locale-setting`\ 을 참고한다.

데이터베이스에 설정된 콜레이션 또는 문자셋을 변환하고 싶으면 해당 칼럼, 테이블, 표현식에 :ref:`COLLATE 수정자<charset-collate-modifier>` 또는 :ref:`CHARSET 수정자<charset-collate-modifier>`\ 를 지정하고, 문자열 상수에 :ref:`COLLATE 수정자<charset-collate-modifier>` 또는 :ref:`charset-introducer`\ 를 지정한다. 이와 관련하여 :ref:`collation-setting`\ 을 참고한다.

문자열 관련 함수나 연산자는 문자셋과 콜레이션에 따라 다르게 동작할 수 있다. 이와 관련하여 :ref:`operations-charset-collation`\ 을 참고한다.


.. toctree::
    :maxdepth: 2

    i18n.rst

