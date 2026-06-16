import io
from datetime import datetime

import numpy as np
import pandas as pd
import plotly.express as px
import requests
import streamlit as st


st.set_page_config(
    page_title="Лабораторна робота №5",
    layout="wide"
)


REGIONS = [
    "Вінницька область",
    "Волинська область",
    "Дніпропетровська область",
    "Донецька область",
    "Житомирська область",
    "Закарпатська область",
    "Запорізька область",
    "Івано-Франківська область",
    "Київська область",
    "Кіровоградська область",
    "Луганська область",
    "Львівська область",
    "Миколаївська область",
    "Одеська область",
    "Полтавська область",
    "Рівненська область",
    "Сумська область",
    "Тернопільська область",
    "Харківська область",
    "Херсонська область",
    "Хмельницька область",
    "Черкаська область",
    "Чернівецька область",
    "Чернігівська область",
    "м. Київ",
]


NOAA_BASE_URL = "https://www.star.nesdis.noaa.gov/smcd/emb/vci/VH/get_TS_admin.php"


def make_demo_dataset() -> pd.DataFrame:
    """Створює демонстраційний датасет з такою ж структурою, як NOAA VCI/TCI/VHI."""
    rng = np.random.default_rng(42)
    rows = []

    for region_id, region_name in enumerate(REGIONS, start=1):
        region_shift = rng.uniform(-8, 8)

        for year in range(1982, 2025):
            for week in range(1, 53):
                seasonal = 50 + 20 * np.sin(2 * np.pi * week / 52)
                trend = (year - 1982) * 0.03

                vci = np.clip(seasonal + region_shift + trend + rng.normal(0, 7), 0, 100)
                tci = np.clip(100 - seasonal + 45 + region_shift / 2 + rng.normal(0, 8), 0, 100)
                vhi = np.clip((vci + tci) / 2 + rng.normal(0, 3), 0, 100)

                rows.append({
                    "Year": year,
                    "Week": week,
                    "Region_ID": region_id,
                    "Region": region_name,
                    "VCI": round(vci, 2),
                    "TCI": round(tci, 2),
                    "VHI": round(vhi, 2),
                })

    return pd.DataFrame(rows)


def parse_noaa_text(text: str, province_id: int, region_name: str) -> pd.DataFrame:
    """Парсить текст NOAA у dataframe."""
    cleaned = (
        text.replace("<pre>", "")
        .replace("</pre>", "")
        .replace("<br>", "\n")
        .replace("</br>", "\n")
        .replace("</tr>", "\n")
    )

    df = pd.read_csv(io.StringIO(cleaned), sep=r"[,;\s]+", engine="python")
    df.columns = [str(c).replace("<", "").replace(">", "").strip() for c in df.columns]

    rename_map = {}
    for col in df.columns:
        lower = col.lower()
        if "year" in lower:
            rename_map[col] = "Year"
        elif "week" in lower:
            rename_map[col] = "Week"
        elif lower == "vci" or "vci" in lower:
            rename_map[col] = "VCI"
        elif lower == "tci" or "tci" in lower:
            rename_map[col] = "TCI"
        elif lower == "vhi" or "vhi" in lower:
            rename_map[col] = "VHI"

    df = df.rename(columns=rename_map)

    required_cols = ["Year", "Week", "VCI", "TCI", "VHI"]
    missing_cols = [col for col in required_cols if col not in df.columns]
    if missing_cols:
        raise ValueError(f"NOAA format error. Missing columns: {missing_cols}")

    df = df[required_cols].copy()

    for col in required_cols:
        df[col] = pd.to_numeric(df[col], errors="coerce")

    df = df.dropna()
    df["Year"] = df["Year"].astype(int)
    df["Week"] = df["Week"].astype(int)
    df["Region_ID"] = province_id
    df["Region"] = region_name

    return df


@st.cache_data(show_spinner=True)
def load_noaa_data() -> pd.DataFrame:
    """Завантажує дані NOAA. Якщо джерело недоступне, повертає demo dataset."""
    frames = []

    try:
        for province_id, region_name in enumerate(REGIONS, start=1):
            params = {
                "country": "UKR",
                "provinceID": province_id,
                "year1": 1982,
                "year2": 2024,
                "type": "Mean",
            }

            response = requests.get(NOAA_BASE_URL, params=params, timeout=15)
            response.raise_for_status()

            frame = parse_noaa_text(response.text, province_id, region_name)
            frames.append(frame)

        result = pd.concat(frames, ignore_index=True)

        if result.empty:
            raise ValueError("Downloaded dataframe is empty.")

        return result

    except Exception:
        return make_demo_dataset()


def reset_filters() -> None:
    """Скидає всі фільтри до початкового стану."""
    st.session_state.metric = "VHI"
    st.session_state.region = REGIONS[0]
    st.session_state.week_range = (1, 52)
    st.session_state.year_range = (1982, 2024)
    st.session_state.sort_ascending = False
    st.session_state.sort_descending = False


def init_state() -> None:
    """Ініціалізує початкові значення інтерактивних елементів."""
    defaults = {
        "metric": "VHI",
        "region": REGIONS[0],
        "week_range": (1, 52),
        "year_range": (1982, 2024),
        "sort_ascending": False,
        "sort_descending": False,
    }

    for key, value in defaults.items():
        if key not in st.session_state:
            st.session_state[key] = value


def filter_dataframe(
    data: pd.DataFrame,
    metric: str,
    region: str,
    week_range: tuple[int, int],
    year_range: tuple[int, int],
    sort_ascending: bool,
    sort_descending: bool,
) -> pd.DataFrame:
    """Фільтрує dataframe за параметрами користувача."""
    filtered = data[
        (data["Region"] == region)
        & (data["Week"].between(week_range[0], week_range[1]))
        & (data["Year"].between(year_range[0], year_range[1]))
    ].copy()

    if sort_ascending and not sort_descending:
        filtered = filtered.sort_values(metric, ascending=True)
    elif sort_descending and not sort_ascending:
        filtered = filtered.sort_values(metric, ascending=False)

    return filtered


init_state()

st.title("Лабораторна робота №5")
st.subheader("Наука про дані: обмін результатами та початковий аналіз")

st.markdown(
    """
**Студент:** Сапронов Анатолій  
**Група:** ФБ-45  
**Дисципліна:** Засоби підготовки та аналізу даних
"""
)

data = load_noaa_data()

min_year = int(data["Year"].min())
max_year = int(data["Year"].max())
min_week = int(data["Week"].min())
max_week = int(data["Week"].max())

with st.sidebar:
    st.header("Фільтри")

    metric = st.selectbox(
        "Оберіть часовий ряд",
        ["VCI", "TCI", "VHI"],
        key="metric",
    )

    region = st.selectbox(
        "Оберіть область",
        sorted(data["Region"].unique()),
        key="region",
    )

    week_range = st.slider(
        "Інтервал тижнів",
        min_value=min_week,
        max_value=max_week,
        value=st.session_state.week_range,
        key="week_range",
    )

    year_range = st.slider(
        "Інтервал років",
        min_value=min_year,
        max_value=max_year,
        value=st.session_state.year_range,
        key="year_range",
    )

    st.markdown("### Сортування")

    sort_ascending = st.checkbox(
        "Сортувати за зростанням",
        key="sort_ascending",
    )

    sort_descending = st.checkbox(
        "Сортувати за спаданням",
        key="sort_descending",
    )

    if sort_ascending and sort_descending:
        st.warning("Обрано обидва типи сортування. Дані будуть показані без сортування.")

    st.button("Reset", on_click=reset_filters)


filtered_data = filter_dataframe(
    data=data,
    metric=metric,
    region=region,
    week_range=week_range,
    year_range=year_range,
    sort_ascending=sort_ascending,
    sort_descending=sort_descending,
)

comparison_data = data[
    (data["Week"].between(week_range[0], week_range[1]))
    & (data["Year"].between(year_range[0], year_range[1]))
].copy()

left_col, right_col = st.columns([1, 2])

with left_col:
    st.markdown("### Таблиця відфільтрованих даних")
    st.dataframe(
        filtered_data[["Year", "Week", "Region", "VCI", "TCI", "VHI"]],
        use_container_width=True,
        height=520,
    )

with right_col:
    st.markdown(f"### Графік {metric} для області: {region}")

    fig_region = px.line(
        filtered_data.sort_values(["Year", "Week"]),
        x="Week",
        y=metric,
        color="Year",
        markers=True,
        title=f"{metric} для {region}",
        labels={"Week": "Тиждень", metric: metric, "Year": "Рік"},
    )

    st.plotly_chart(fig_region, use_container_width=True)

    st.markdown(f"### Порівняння {metric} для {region} з іншими областями")

    comparison_mean = (
        comparison_data
        .groupby("Region", as_index=False)[metric]
        .mean()
        .sort_values(metric, ascending=False)
    )

    comparison_mean["Selected"] = np.where(
        comparison_mean["Region"] == region,
        "Обрана область",
        "Інші області",
    )

    fig_comparison = px.bar(
        comparison_mean,
        x="Region",
        y=metric,
        color="Selected",
        title=f"Середнє значення {metric} по областях за вибраний період",
        labels={"Region": "Область", metric: f"Середнє {metric}"},
    )

    fig_comparison.update_layout(xaxis_tickangle=-45)
    st.plotly_chart(fig_comparison, use_container_width=True)


st.markdown("### Опис")
st.write(
    """
Інтерактивні елементи керування розміщені у лівій колонці/sidebar, 
а таблиця та графіки — в основній частині веб-додатку. 
Перший графік показує часовий ряд для вибраної області. 
Другий графік порівнює середні значення обраного показника між областями України.
"""
)
