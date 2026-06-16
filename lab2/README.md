# Лабораторна робота №2

**Студент:** Сапронов Анатолій  
**Група:** ФБ-45  
**Дисципліна:** Засоби підготовки та аналізу даних

## Тема
Наука про дані: підготовчий етап.

## Структура
- `lab2_part1.ipynb` — робота з NOAA VHI/VCI даними по областях України.
- `lab2_part2.ipynb` — аналіз датасету Individual Household Electric Power Consumption.
- `requirements.txt` — потрібні Python-пакети.
- `.gitignore` — виключення службових файлів, кешу та датасетів.

## Встановлення
```bash
python -m venv .venv
source .venv/bin/activate      # Linux / macOS
# або
.venv\Scripts\activate       # Windows

pip install -r requirements.txt
```

## Запуск
```bash
jupyter notebook
```

Після запуску відкрийте потрібний `.ipynb` файл та виконуйте клітинки зверху вниз.

## Примітка
Дані завантажуються автоматично під час виконання ноутбуків. Завантажені `.txt`, `.csv`, `.json`, `.zip` файли не додаються в GitHub, бо вони прописані в `.gitignore`.
