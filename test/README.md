# HOW TO TEST

You will need Python 3 (ideally Python 3.5) and libraries from requirements.txt

## Configuration

```
pip install -r requirements.txt
```

## Testing

### Everything in the current directory

```
python -m unittest discover
```

## Everything in a file

```
python -m unittest <file name>
```

## Specific test case in a file

```
python -m unittest <file name> <testcase name>
```