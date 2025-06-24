import argparse
from . import __version__


def main(argv=None):
    parser = argparse.ArgumentParser(
        description="SDK winK - Wine toolkit for running Windows games on Arch Linux"
    )
    parser.add_argument("--version", action="version", version=__version__)

    subparsers = parser.add_subparsers(dest="command")

    setup_parser = subparsers.add_parser("setup", help="Initialize a Wine prefix")
    setup_parser.add_argument(
        "--prefix",
        default="~/.wine-wink",
        help="Path to the Wine prefix (default: ~/.wine-wink)",
    )

    args = parser.parse_args(argv)

    if args.command == "setup":
        print(f"Setting up Wine prefix at {args.prefix}")
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
