# Лабораторна робота №3

## Візуалізація даних

**Студент:** Сапронов Анатолій  
**Група:** ФБ-45  
**Дисципліна:** Засоби підготовки та аналізу даних

## Тема

Візуалізація даних у Python з використанням `pandas`, `numpy` та `matplotlib`.

## Мета роботи

Метою роботи є вибір унікального multivariate датасету, очищення даних та побудова графіків, які показують залежності між ознаками, розподіл значень і корисну інформацію про набір даних.

## Датасет

Для роботи використано **Adult Dataset**.

Датасет відповідає вимогам:

- Dataset Characteristics: Multivariate;
- Attribute Characteristics: Categorical, Integer, Real;
- Number of Attributes: більше двох integer/real атрибутів;
- Has Missing Values: Yes.

## Виконані завдання

У файлі `lab3.ipynb` виконано:

1. Вибір датасету.
2. Data Cleaning.
3. Побудова scatter plot.
4. Побудова histogram з 5 діапазонами.
5. Побудова line plot.
6. Побудова bar chart.
7. Побудова box plot.
8. Побудова correlation heatmap.
9. Побудова додаткового корисного графіка.

## Структура

- `lab3.ipynb` — Jupyter Notebook з виконаною лабораторною роботою.
- `README.md` — опис роботи.
- `requirements.txt` — залежності.

## Запуск

```bash
python -m venv .venv
```

```bash
.venv\Scripts\activate
```

```bash
pip install -r requirements.txt
```

```bash
jupyter notebook
```

