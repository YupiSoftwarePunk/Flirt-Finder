import requests
from bs4 import BeautifulSoup
import psycopg2


# Database configuration
DB_CONFIG = {
    'dbname': 'cities',
    'user': 'postgres',
    'password': 'C0d38_50AdM1Nn6',
    'host': 'localhost',
    'port': '5432'
}

# Wikipedia URL
URL = "https://ru.wikipedia.org/wiki/%D0%A1%D0%BF%D0%B8%D1%81%D0%BE%D0%BA_%D0%B3%D0%BE%D1%80%D0%BE%D0%B4%D0%BE%D0%B2_%D0%A0%D0%BE%D1%81%D1%81%D0%B8%D0%B8"


response = requests.get(URL)
response.encoding = 'utf-8'  # Явно устанавливаем правильную кодировку
response.raise_for_status()
soup = BeautifulSoup(response.text, 'html.parser')


def get_cities_from_wikipedia(url):
    """Fetches city names from Wikipedia"""
    response = requests.get(url)
    response.raise_for_status()  # Raise error if request fails

    soup = BeautifulSoup(response.text, 'html.parser')
    tables = soup.find_all('table', class_='wikitable')  # Locate tables with class 'wikitable'

    cities = []
    for table in tables:
        rows = table.find_all('tr')
        for row in rows:
            cells = row.find_all('td')
            if len(cells) > 1:  # Ensure row has necessary cells
                city_name = cells[1].get_text(strip=True)  # Second column: city name
                cities.append(city_name)

    return list(set(cities))  # Remove duplicates

def insert_cities_into_db(cities):
    """Inserts city names into PostgreSQL database"""
    try:
        conn = psycopg2.connect(**DB_CONFIG)
        cursor = conn.cursor()

        # Insert cities into the table
        for city in cities:
            cursor.execute(
                "INSERT INTO cities (name) VALUES (%s) ON CONFLICT DO NOTHING",
                (city,)
            )
        conn.commit()
        print(f"{len(cities)} cities have been successfully added to the database!")

    except Exception as e:
        print(f"Database error: {e}")
        return

    finally:
        if conn:
            cursor.close()
            conn.close()

def main():
    # Fetch cities from Wikipedia
    print("Fetching city names from Wikipedia...")
    try:
        cities = get_cities_from_wikipedia(URL)
        if not cities:
            print("No cities found. Please check the parsing logic or URL.")
            return
        print(f"Found {len(cities)} cities. Inserting into the database...")

        # Insert cities into the database
        insert_cities_into_db(cities)
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
