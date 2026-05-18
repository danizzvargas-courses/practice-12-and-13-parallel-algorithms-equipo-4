# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

EDA II – Parallel Algorithms exposition using Python. Implementations go in `src/pruebas.py` and tests in `test/test.py`.

## Setup

```bash
pip install -r requeriments.txt
```

## Run

```bash
python src/pruebas.py
```

## Testing

Uses `pytest`. Run all tests:

```bash
pytest test/ -v
```

Run a single test:

```bash
pytest test/test.py::test_ejemplo -v
```

## Structure

- `src/pruebas.py` — main implementation file
- `test/test.py` — imports from `src.pruebas` and contains pytest functions
- `src/__init__.py` — makes `src` importable as a package
