import requests
import json
import sys

# API key
API_KEY = "d963abb4f77678a4c29cc081aed64f56"

def fetch_flights(source, destination):
    # API endpoint
    url = "http://api.aviationstack.com/v1/flights"

    # API parameters
    params = {
        'access_key': API_KEY,
        'dep_iata': source.upper(),
        'arr_iata': destination.upper(),
        'limit': 10,  # Limit to 10 flights for simplicity
    }

    # Make the API request
    response = requests.get(url, params=params)

    if response.status_code == 200:
        data = response.json()

        # Filter and save relevant flight details
        flights = []
        for flight in data.get('data', []):
            flight_info = {
                'flight_number': flight.get('flight', {}).get('iata', "N/A"),
                'departure': flight.get('departure', {}).get('iata', "N/A"),
                'arrival': flight.get('arrival', {}).get('iata', "N/A"),
                'status': flight.get('flight_status', "N/A"),
            }
            flights.append(flight_info)

        # Output the flights as JSON
        print(json.dumps(flights, indent=4))
    else:
        print(f"Error: Unable to fetch flights (Status Code: {response.status_code})", file=sys.stderr)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python fetch_flights.py <source> <destination>", file=sys.stderr)
        sys.exit(1)

    source = sys.argv[1]
    destination = sys.argv[2]
    fetch_flights(source, destination)
