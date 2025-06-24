import os
import sys

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from wink import cli


def test_cli_help(capsys):
    try:
        cli.main(["--help"])
    except SystemExit:
        pass
    captured = capsys.readouterr()
    assert "SDK winK" in captured.out
