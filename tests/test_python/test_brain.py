
import detectmateperformance.brain as brain
from detectmateperformance.lib.bind_class import brain as native_brain

import polars as pl


class TestBrain:
    def test_native_brain_accepts_arrow_table(self) -> None:
        table = pl.DataFrame({
            "Lenght": [6, 5, 3],
            "Template": [
                [
                    {'position': 0, 'word': 'INFO'},
                    {'position': 1, 'word': 'user'},
                    {'position': 3, 'word': 'login'},
                    {'position': 4, 'word': 'succeeded'},
                    {'position': 5, 'word': 'CIAO'}
                ],
                [
                    {'position': 0, 'word': 'INFO'},
                    {'position': 1, 'word': 'disk'},
                    {'position': 3, 'word': 'became'},
                    {'position': 4, 'word': 'full'}
                ],
                [
                    {'position': 0, 'word': 'hello'},
                    {'position': 1, 'word': 'there'}
                ]
            ]
        })

        expected = pl.DataFrame({
            "Templates": [
                "INFO user <*> login succeeded CIAO",
                "INFO disk <*> became full",
                "hello there <*>"
            ]
        })

        results = pl.from_arrow(native_brain(table.to_arrow()))
        assert results.equals(expected)

    def test_cluster_logs(self):
        logs = pl.DataFrame({
            "Content": [
                "INFO user alice login succeeded CIAO",
                "INFO user bob login succeeded CIAO",
                "INFO user carol login succeeded CIAO",
                "INFO disk sda became full",
                "INFO disk sdb became full",
                "hello there"
            ],
        })

        results = brain.cluter_logs(logs, weight=0.3).sort(
            by="Lenght", descending=True
        )
        expected = pl.DataFrame({
            "Lenght": [6, 5, 2],
            "Template": [
                [
                    {'position': 0, 'word': 'INFO'},
                    {'position': 1, 'word': 'user'},
                    {'position': 3, 'word': 'login'},
                    {'position': 4, 'word': 'succeeded'},
                    {'position': 5, 'word': 'CIAO'}
                ],
                [
                    {'position': 0, 'word': 'INFO'},
                    {'position': 1, 'word': 'disk'},
                    {'position': 3, 'word': 'became'},
                    {'position': 4, 'word': 'full'}
                ],
                [
                    {'position': 0, 'word': 'hello'},
                    {'position': 1, 'word': 'there'}
                ]
            ]
        })
        assert results.equals(expected)

    def test_brain_pipeline(self):
        drain_ = brain.Brain(weight=0.5)
        regex = r"type=(?P<Type>\w+) msg=audit\((?P<Time>[^:]+):(?P<Serial>\d+)\): (?P<Content>.*)"

        templates = drain_(logs="tests/test_data/audit.log", regex=regex).get_templates(False)
        assert len(templates) == 10
