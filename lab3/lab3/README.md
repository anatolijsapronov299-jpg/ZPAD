# Лабораторна робота №3

**Студент:** Сапронов Анатолій  
**Група:** ФБ-45  
**Дисципліна:** Засоби підготовки та аналізу даних

## Тема
Візуалізація даних.

## Датасет
Для роботи використано **Adult Dataset** з UCI Machine Learning Repository.

Датасет відповідає вимогам:
- Dataset Characteristics: Multivariate
- Attribute Characteristics: Categorical, Integer, Real
- Number of Attributes: більше 2 integer/real
- Has Missing Values: Yes

## Структура
- `lab3.ipynb` — основний Jupyter Notebook з виконанням лабораторної роботи.
- `requirements.txt` — потрібні Python-бібліотеки.
- `.gitignore` — виключення службових файлів, кешу та датасету.

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

Після запуску відкрийте `lab3.ipynb` та виконуйте клітинки зверху вниз.

## Що виконано
- Завантажено Adult Dataset.
- Проведено data cleaning.
- Побудовано 8 графіків:
  1. Scatter plot
  2. Line plot
  3. Histogram
  4. Bar chart
  5. Box plot
  6. Correlation heatmap
  7. Parallel coordinates plot
  8. Scatter matrix
