# Sorted Set in Redis

## Introduction
A sorted set in Redis is a collection of unique elements, where each element is associated with a score. 
The elements are sorted by their scores in ascending order. This data structure is useful for scenarios where 
you need to maintain a sorted list of items and perform operations like range queries, leaderboard rankings, etc.

## Applications
1. **Leaderboards**: Sorted sets are commonly used to implement leaderboards in gaming applications. Players' scores can be stored in a sorted set, and the top players can be retrieved efficiently.
2. **Priority Queues**: Tasks with different priorities can be managed using sorted sets. Tasks with higher priority can be processed first.
3. **Ranking Systems**: Sorted sets can be used to rank items based on their scores, such as ranking products based on user ratings.
4. **Time-Series Data**: Sorted sets can store time-series data where the score represents the timestamp, allowing efficient range queries over time intervals.

Big-O of Sorted Set:

    Find the score by name: O(1) by a hashtable.
    Insert a pair: O(log(n)).
    Range query: O(log(n)).
    Rank query:
    Find a pair’s rank: O(log(n)).
    Find by rank: O(log(n)).
    Offsetting: O(log(offset)).