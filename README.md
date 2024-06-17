# Pokémon FireRed and LeafGreen for Archipelago

This is a fork of the [pret decompilation of Pokémon FireRed and LeafGreen](https://github.com/pret/pokefirered). It is meant for use with the [Archipelago mutliworld randomizer](https://archipelago.gg/). On its own, the ROMs it builds will appear to be vanilla FireRed or LeafGreen with some quality of life changes.

To set up the repository, see [INSTALL.md](INSTALL.md).

## What's Different?

Much of the work is in `tools/extractor`, which pulls addresses and data from source files, the linker map, and the binary. Many changes to the source code enable this tool to pull out relevant data. The extracted information is then used by the randomizer to create items and patch the ROM after randomization.

There are also many tweaks and QoL changes to make the specific experience of playing a randomizer more enjoyable and intuitive.
