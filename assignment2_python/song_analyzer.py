
#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""

Song Analyzer 

This script analyzes data from a csv file containing information about songs and generates an 
output based on the provided command-line arguments.

Based on: https://www.kaggle.com/datasets/nelgiriyewithana/top-spotify-songs-2023

Sample input: --filter="ARTIST" --value="Dua Lipa" --order_by="STREAMS" --order="ASC" --limit="6"'

"""

import csv
from datetime import datetime
from typing import List, Optional, Any
import argparse

# Define constants
DATA_FILE = "data.csv"
OUTPUT_FILE = "output.csv"

class Song:
    """
    Song class represents a song with various attributes.
    """
    def __init__(self, track_name: str, artist_names: str, artist_count: int, released_year: int, released_month: int, released_day: int, 
                 in_spotify_playlists: int, streams: int, in_apple_playlists: int, bpm: int, key: Any, mode: Any, danceability: int) -> None:
        self.track_name = track_name
        self.artist_names = artist_names
        self.artist_count = artist_count
        self.released_year = released_year
        self.released_month = released_month
        self.released_day = released_day
        self.in_spotify_playlists = in_spotify_playlists
        self.streams = streams
        self.in_apple_playlists = in_apple_playlists
        self.bpm = bpm
        self.key = key
        self.mode = mode
        self.danceability = danceability


def parse_command_line() -> tuple:
    """
    Parse command-line arguments.

    Returns:
    tuple: A tuple containing the parsed command-line arguments.
    """
    parser = argparse.ArgumentParser(description="Song Analyzer Command Line Parser")
    parser.add_argument("--data", help="Path to the data file", required=True)
    parser.add_argument("--filter", help="Filter criteria", required=False)
    parser.add_argument("--value", help="Value for the filter", required=False)
    parser.add_argument("--order_by", help="Field to order by", required=False)
    parser.add_argument("--order", help="Order (ASC or DES)", required=False)
    parser.add_argument("--limit", help="Limit number of results", required=False)
    
    args = parser.parse_args()
    
    return args.data, args.filter, args.value, args.order_by, args.order, args.limit


def read_data(filename: str) -> List[Song]:
    """
    Reads data from a csv file and parses it into songs objects.

    Parameters:
    filename (str): The name of the csv to read.

    Returns:
    list: A list of Song objects parsed from the CSV file.
    """
    songs = []
    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        next(reader) 
        for row in reader:
            track_name, artist_names, artist_count, released_year, released_month, released_day, \
                in_spotify_playlists, streams, in_apple_playlists, bpm, key, mode, danceability = row
            
            song = Song(track_name, artist_names, int(artist_count), int(released_year), int(released_month), 
                        int(released_day), int(in_spotify_playlists), int(streams), int(in_apple_playlists), 
                        int(bpm), key, mode, int(danceability))
            
            songs.append(song)
    
    return songs


def write_output(output: List[Song], filename: str, ordered_by: str) -> None:
    """
    Writes the output data to a csv file.

    Parameters:
    output (list): the list of output data.
    filename (str): The name of the output CSV file.
    ordered_by (str): the column to order the output data by.
    """
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        
        # Write CSV header based on the ordered_by column
        if ordered_by == 'STREAMS':
            writer.writerow(['released', 'track_name', 'artist(s)_name', 'streams'])
        elif ordered_by == 'NO_SPOTIFY_PLAYLISTS':
            writer.writerow(['released', 'track_name', 'artist(s)_name', 'in_spotify_playlists'])
        elif ordered_by == 'NO_APPLE_PLAYLISTS':
            writer.writerow(['released', 'track_name', 'artist(s)_name', 'in_apple_playlists'])

        # Write data to CSV
        for song in output:
            released_date = datetime(year=song.released_year, month=song.released_month, day=song.released_day).strftime("%a, %B %d, %Y")
            if ordered_by == 'STREAMS':
                writer.writerow([released_date, song.track_name, song.artist_names, song.streams])
            elif ordered_by == 'NO_SPOTIFY_PLAYLISTS':
                writer.writerow([released_date, song.track_name, song.artist_names, song.in_spotify_playlists])
            elif ordered_by == 'NO_APPLE_PLAYLISTS':
                writer.writerow([released_date, song.track_name, song.artist_names, song.in_apple_playlists])


def apply_filter(songs: List[Song], filter_criteria: Optional[str], value: Optional[str]) -> List[Song]:
    """
    Apply filter to the list of songs based on filter criteria and value.

    Parameters:
    songs (List[Song]): List of songs to filter.
    filter_criteria (Optional[str]): Column used to filter rows.
    value (Optional[str]): Specific value to filter rows using the filter_criteria.

    Returns:
    List[Song]: Filtered list of songs.
    """
    if filter_criteria is None or value is None:
        return songs
    
    filtered_songs = []
    for song in songs:
        if filter_criteria == 'ARTIST' and value in song.artist_names:
            filtered_songs.append(song)
        elif filter_criteria == 'STREAMS' and str(value) == str(song.streams):
            filtered_songs.append(song)
        elif filter_criteria == 'YEAR' and str(value) == str(song.released_year):
            filtered_songs.append(song)

    return filtered_songs

def order_data(songs: List[Song], order_by: Optional[str], order: Optional[str]) -> List[Song]:
    """
    Order the list of songs based on the specified order_by column and order type.

    Parameters:
    songs (List[Song]): List of songs to order.
    order_by (Optional[str]): Column used to sort the rows.
    order (Optional[str]): Type of order the data will follow (ASC or DES).

    Returns:
    List[Song]: Ordered list of songs.
    """
    if order_by is None or order is None:
        return songs
    
    # Map command-line argument names to Song attribute names
    attribute_mapping = {
        'STREAMS': 'streams',
        'NO_SPOTIFY_PLAYLISTS': 'in_spotify_playlists',
        'NO_APPLE_PLAYLISTS': 'in_apple_playlists'
    }
    
    # Get the corresponding attribute name for the provided order_by value
    attribute_name = attribute_mapping.get(order_by, None)
    
    if attribute_name is None:
        raise ValueError(f"Invalid order_by value: {order_by}")
    
    # Sort the songs based on the specified attribute and order
    if order.upper() == 'ASC':
        ordered_songs = sorted(songs, key=lambda x: getattr(x, attribute_name))
    elif order.upper() == 'DES':
        ordered_songs = sorted(songs, key=lambda x: getattr(x, attribute_name), reverse=True)
    else:
        raise ValueError(f"Invalid order value: {order}")
    
    return ordered_songs


def limit_data(songs: List[Song], limit: Optional[int]) -> List[Song]:
    """
    Limit the number of songs displayed.

    Parameters:
    songs (List[Song]): List of songs to limit.
    limit (Optional[int]): Number of songs to limit.

    Returns:
    List[Song]: Limited list of songs.
    """
    if limit is None:
        return songs
    
    return songs[:limit]


def main() -> None:
    """
    Main function to execute the program.

    This function reads command-line arguments, reads data from a CSV file, and answers the specified question.
    """
    data_file, filter_criterion, value, order_by, order, limit = parse_command_line()
    
    songs: List[Song] = read_data(data_file)
    
    filtered_songs = apply_filter(songs, filter_criterion, value)
    
    ordered_songs = order_data(filtered_songs, order_by, order)
    
    if limit:
        ordered_songs = ordered_songs[:int(limit)]
    
    write_output(ordered_songs, OUTPUT_FILE, order_by)


if __name__ == '__main__':
    main()

