
from detectmateperformance._iparser import clean_strings

import polars as pl


def test_clean_strings():
    result = clean_strings(pl.DataFrame({"Content": [
        "Hello, world",
        "Hello. world 123 21",
        "Hello. world 129.131.12.12",
        "Receiving block blk_-1608999687919862906",
        "Receiving block /home/linux/home/windows",
        "Receiving src: /123.1 asdasd aas ",
        "proctitle 61707061726D6F725F706172736572002D",
    ]}))["Content"].to_list()

    expected = [
        "Hello world",
        "Hello world VAR",
        "Hello world VAR",
        "Receiving block blk_-1608999687919862906",
        "Receiving block VAR",
        "Receiving src VAR asdasd aas",
        "proctitle VAR",
    ]

    assert result == expected
