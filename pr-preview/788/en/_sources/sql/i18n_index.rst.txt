
:meta-keywords: cubrid localization, cubrid locale, cubrid internationalization, cubrid timezone, cubrid i18n, cubrid ldml
:meta-description: Globalization includes internationalization and localization. Internationalization can be applied to various languages and regions. Localization fits the language and culture in a specific area as appending the language-specific components. CUBRID supports multilingual collations including Europe and Asia to facilitate the localization.

*************
Globalization
*************

Globalization includes internationalization and localization. Internationalization can be applied to various languages and regions. Localization fits the language and culture in a specific area as appending the language-specific components. CUBRID supports multilingual collations including Europe and Asia to facilitate the localization.

If you want to know overall information about character data setting, see :ref:`char-data-conf-guide`.

If you want to know about charset, collation and locale, see :ref:`globalization-overview`.

For timezone type and realted functions, see :ref:`timezone-type`. For timezone related system parameters, see :ref:`timezone-parameters`. If you want to update a timezone information as a new one, timezone library should be recompiled; for details, see :ref:`timezone-library`.

If you want to apply the wanted locale to the database, you have to set the locale firstly, then create the database. Regarding this setting, see :ref:`locale-setting`.

If you want to change the collation or charset specified on the database, specify :ref:`COLLATE modifier <charset-collate-modifier>` or :ref:`CHARSET modifier <charset-collate-modifier>` to the column, table, expression, and specify :ref:`COLLATE modifier <charset-collate-modifier>` or :ref:`charset-introducer` to the string literal. Regarding this setting, see :ref:`collation-setting`.

The functions or operators related to strings can work differently by charset and collation. Regarding this, see :ref:`operations-charset-collation`.

.. toctree::
    :maxdepth: 2

    i18n.rst

