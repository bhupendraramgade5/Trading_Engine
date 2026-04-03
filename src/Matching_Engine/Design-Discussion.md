## 1. Should We Match Before Inserting?

        why do we try to match first when we havent even inserted the order into book doesnt this break the consistency

`In real exchanges, match-before-insert is correct`  This is called: `Aggressive Order vs Passive Order`
```ts
        Incoming order => Aggressive
        Existing orders => Passive

```

        Example:

        Order Book:

        SELL

        100 -> 50 qty

        Incoming:

        BUY 100 qty @ 100

What happens?

Correct behaviour: `Match immediately Only leftover goes to book`

So:

        match 50
        remaining 50 → insert into BUY book

This is standard exchange behavior.

## Matching Engine Responsibilities
### Core Responsibilities

* ✔ Parse commands (via parser)
* ✔ Route commands to OrderBook
* ✔ Create OrderBook per symbol
* ✔ Handle trade callback
* ✔ Manage orderbook lifecycle

###  Future Responsibilities
* ✔ Connect PositionEngine
* ✔ Connect RiskEngine
* ✔ Multi-threading
* ✔ Logging