1. Should We Match Before Inserting?

You asked:

why do we try to match first when we havent even inserted the order into book doesnt this break the consistency

This is a very good observation, and here's the correct answer:

In real exchanges, match-before-insert is correct

This is called: `Aggressive Order vs Passive Order`

Incoming order = Aggressive
Existing orders = Passive

        Example:

        Order Book:

        SELL

        100 -> 50 qty

        Incoming:

        BUY 100 qty @ 100

What happens?

Correct behaviour:

Match immediately
Only leftover goes to book

So:

match 50
remaining 50 → insert into BUY book

This is standard exchange behavior.