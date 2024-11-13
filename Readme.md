# Computer Networks Laboratory

## (ENGR-UH 3512\) Fall 2024

**Assignment 10:** Forwarding and Routing  
**Deadline:** November 15th, 2024  
---

# 0\. Overview

The control plane is a separate infrastructure that is not accessible directly via end users like us. The data that travels across the data plane contains routing information, configuration settings for routers and other data that is not usable for end-users.

For more efficiency and increased performance, the routing tables of all routers on the internet must be updated periodically so that packets can be forwarded through the links with the lowest latency.

In computer science, computing routing tables is akin to finding the shortest path in a connected graph. For larger networks, this problem becomes exponentially harder and almost intractable. That said, for a small network, the problem is trivial (in terms of computation).

There are many algorithms for finding shortest paths in a connected graph, you are already familiar with one of them: Dijkstra’s algorithm. In today’s lab, we will be looking at an algorithm from another family of routing protocols called: distance vector routing protocols.

# 1\. Path Finding Algorithms

## 1.1 Link-State Algorithms vs Distance Vector Algorithms

Link state algorithms, an example of which is the Open Shortest Path First or OSPF algorithm which relies on Dijkstra’s algorithm to establish the routes. The idea behind link-state routing algorithms is to give every node/router in the system a complete picture of the network by computing the shortest paths from every node to every other node.

The other classification of algorithms is the distance-vector algorithm. This class of algorithms is different in that the routers implementing these kinds of algorithms rely on local knowledge from immediate neighboring nodes to generate their routing tables, and do not establish the full picture.

There are other differences between the two algorithms, see table 1 below.

Table 1\. Differences between distance vector and link-state routing algorithms 

| Distance Vector Routing | Link-State Routing  |
| :---: | :---: |
| Less bandwidth required due to local sharing, small packets and no flooding. | More bandwidth required due to flooding and sending of large link state packets. |
| Based on local knowledge, since it updates tables based on information from neighbors.  | Based on global knowledge, it has knowledge about the entire network. |
| Make use of Bellman Ford Algorithm. | Make use of Dijakstra’s algorithm. |
| Less traffic. | More traffic |
| Converges slowly | Converges faster. |
| Count of infinity problem[^1]. | No count of infinity problems1. |
| Persistent looping problem. | No persistent loops, only transient loops. |
| Practical implementation is RIP and IGRP. | Practical implementation is OSPF and ISIS. |

## 1.2 Usage in Networks

Generally speaking, link-state algorithms are more suitable for larger networks. Because link-state algorithms converge much quicker than distance vector algorithms, they are better suited for the task.

Link-state algorithms are also more suitable as they provide more precise pathways for the routers to follow, they can avoid looping problems as each router only needs to update their internal metrics’ tables (in the case a link goes down or some other network error occurs), then reuse Dijakstra’s algorithm to recompute the routing table quickly.

# 2\. Distributed Asynchronous Distance Vector Routing

Distance vector routing algorithms utilize the Bellman Ford algorithm which can be simplified into the following equation:

Dx(y)=min{cx,v+Dv(y)}

Where *Dx(y)* denotes the distance between node x and y. To compute this distance, the total distance connecting x to a neighboring node v is added to the distance of node v to the destination node y. This computation is redone for all neighboring nodes and the minimum distance is the one that is chosen.

In order to do this computation, a node x must know the distance between node v and node y, but given that it is a different node/router, node x does not have direct access to this information, that is why routers implementing distance vector algorithms also “advertise” their minimum costs to their neighbors, when the minimum cost between in one node’s routing table changes, it is advertised to all neighboring nodes.

The algorithm ultimately reaches convergence where no routers routing table is updated (assuming there is not a network failure or any extenuating circumstances). However, due to its asynchronous nature, and the necessity for many advertisements, the convergence is slow.

Furthermore, because advertisements are only made when the minimum cost changes, a link failure could go undetected for some time before it is finally caught.

# 3\. Tasks

In today’s lab, we will be using C to implement a simplified version of this routing algorithm in a simulation environment.

![][image1]  
*Figure 1\. Network topology in this simulation.*

## 3.1 Task \#1

Download the starter code from Brightspace, the code contains three files:

1. distance\_vector.c: this is the C code responsible for running the simulation environment. There should be no need to modify this file.  
2. nodeN: 4 files, titled node0, node1, … each file represents a router/node. These files must be modified.

## 3.2 task \#2

Implement the *rtinitN()* functions, these functions are supposed to initialize the distance tables in each router initializing the distances in the dt0, dt1, … variables.

After initialization of the distance table, the function sends an initial distance vector to its neighbors by using the function *tolayer2()*.

## 3.3 Task \#3

Implement the *rtupdateN()* functions, these functions are responsible for handling arriving distance vectors and update their own internal distance tables.

Note that all entries within the distance table must be filled, not just the minimum distance.

[^1]:  The Count to Infinity problem occurs when routers keep updating their routing tables with wrong inflated values mostly due to failures in the network which ends up making routers count distance to destination as infinity hence resulting in slow convergence and routing loops.

[image1]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAU4AAADjCAYAAADqpv3tAABbJUlEQVR4Xu19B4AVNdd2fPX157O86ucndtBXbEjvLL33Lh2kigKigiCiIIpUFVSKgggISke69C4gilRBkF6liPS+uzx/TnLP3Gz23r3bG3mWw8ydyWSSk5MnJ5lMRsDBwcHBIU4Q9gEHBwcHh5jhiNPBwcEhjnDE6eDg4BBHOOJ0cHBwiCMccTo4ODjEEY44HRwcHOIIR5wODg4OcYQjTgcHB4c4whGng4ODQxzhiNPBwcEhjnDE6eDg4BBHOOJ0cHBwiCMccTo4ODjEEemCOG/cuOFt7f1AYiPYNeaxQPsOCcAN+kf/RS8fLU7HDqkXaZ44AxFZqP1ABGges39HrdB+cYg/lPo0b/oOWGXg9OuQipFqiDNGkuLj/GccD3mtlMjISG8bSuxrTTHvxbDDRBUVIEo43r954dOLryz1b70fRf++4zczTHsx7cc8Fu08/VP75nFthma4YHHacZu/Yyu+RPiPGXU2UHx8LC0hRYkzmCL9Ev0cVa6IiAgl4eHhuH79uhLeDw+X54gEKbx1v7iCrqd49L0ivHuZ96RzMZNu9DyYcrMhql58OqMylbr0h6Hjaa8yJTY4/7bNxEfYRrnumHXIFNO2TbGvi62zEVsx85sWkGLEGUhZrEQuYLPQ4qPTG+HXcfXiOZw7eULJicMHcfzgfpyQorcHfKJ///PXURXu/Km/cePqFcjabEcZEpR2le7r/rSbxmUj0LF0DV92lT7oT+rm6tWr6tjMEb3RuPW72gak7iMlgd7sCGQfweqJ3ZhHvzLpQPeKjGQnw5+W2NQBQrDjqRVJRpwmMcYkTDSkaFJwTLhy/hyO7N6JzatXYcWs6fhu8Gf47P338GHnjnjj5daoU6USKpUtjdKlSqFw2QrIXaYcnilYFA9mz4uMOfLhnhdy4Z7s+fCfHPmjyT3Z8uL/Xsitwj2cPQ9ylCyDfKXKoXi58ihXsgSqVSiHZvXq4Z0O7dHvnc4Y1vsDzB43Br8sXoidG3/FmWNHESmJOhjIc71uGBQbEsHWiSnpDTeMbhsRI+dww5LJ+F8hULhyW/U7QuryRmT6y78JVb4BytwWk4xiU088SP1eu3QRF6QzcHTPLuzatB5b167Gr0sXY+38uVg+8wfMm/gd5k/8HgsnT8B3Qz7HiAF9pfTB2MGD5LHxWDDpe8yb8B2WzZgmr/lRXrtIxvET/tywXtbFP3HuxDF5j/MqH7EBpd3usdn5DSQE3qYGJAlxBsso/fY8MimB1HDt8kUc+GMbFv8wGSM/7Y9OrVqgfp3ayFugoCTBIrgtXzGIXFLKNYKo2gaizpsQbQdCvPsdRO/pEJ8sgvjyZ4jxf0JMOwgx6zjEwvMQiy5ALLkEsTJcSoSx9cny6zrM4osQ805DTP8LYvI+iG9/h/h8OUT/HyF6ToZ462uIpj0garaDqNwaomhVmZ4wPBxWGtnyF0D5CuXxSpOG+KhLR0wf8w02rlyO08f+srOpEBER6VUEU1e23tITuCKwt3Tx7En069YeD9xzHx57+GGUr/26Pi89zvSsB4aZQ1MvXEeC4fqVyzh5+BB+lyQ2TxLfuCGfoWfHDmj/UmM0adAAJUuXRs58+fFsoaJ4vFg5ZKB6kzMMIrfc5i8LUaEJRPmGsg61gqj+CkQNWZcqtJD16iUtFZvrYyTVZJhyDfU1BcrIOIqruG7PWwyPybjpHjnzFUDREsXRuH49vCbT8IFMC6WJSHnrmlUqreEyzcHAhGp6p6kZiUKcMbUObAykFHLlTVDl2LNlM6Z9MwI9Xm+P6hUr4tkCYbg1b1GIIpKQ6r6hSbHvHElgWyFmn4BYLRO9ScoOKXuk7PNt/5Sy03d8m5TNUjZK+U3Kr4asCyJmmPW+6+g+W6Rsh46X4t8FfU8Suifdi8IvuQwxaS/EsLUQb4+BaN5LGmZTiMIV8WCuAihcogSa13tReqq9sFq29qePRydTr0Gx9OnplbYBm5u0A/Ii6Y/sgTDnyx54NNMzmLt6KzrWL4u8pZup4+mJOHXZRa8nLB5pREb3JCOuXcV+5UhMwVBpO21faoLKFSri+UJFcH/uQrKeVJZSHeJFWVda9oZ48yvZyM+DGP4LxPd/asdhsXQYfvLZNtn07z57JiEbZqF6ZIp5jsPTtVS3yOapLpLd0z3G74QYsR5iwHyIjl9CtOqj00Rpk2mktGaVaa5SsZLMQ2NVDyhPlLfwa3qoJkq+fY1HQI8UKe9cJJg4vUrt2zePhfsyb+Lgzj/w/bAv0LphfeTMX1C2XkVkayZbslcGQHy6AGLGEV0gRFJcgLRPBEZESCS31ldoJD9F+vdZ1viEwiVEOB4SM366J9+X70NGSQZFRErkSummLR0jL3bMZoj3vtceclgVPJq3ECqVL4e+XTpJIv0Rly+cj6In3dBEehXO48uUtZcEQxHnDX6CHomTRw/h7AVNoq/WKIr8ZVqq/fRCnF6dUAXpP8bOhJ1F6p2smD0T/bt2Qb2a1ZG9YBhuyVUYotSLkog6QnQdrXtAPxyGWHpFE6FNerbzwA6Dadd2nYmtmHWLHQ66B92LCZnSsNe3pbRRGpddlWk+JNO+TDoWo3VeZJ7+lbOwymO9GtVVninvdg+NhrlMEiWlMXmmlI0kmDgZJmkqd9sY2N/3x3Z8/mFPlClVCv8vr+wuVGwG8c63skXcAfGzT7lc4FQIdCwYQaVmMUmW006/ybiI+CmfZFBkYAvPSiNaIrv970IULI9n8hXEK40bYsn0abjue1hC8DxQ+vNtCbxNa4iSF5/NkKdFNtO2ZnHkL9tCHUsPxGnWCW4AKZ/2GOXmNT9h4Pvvoqr0JB/OLZ2JwtKLbNhV9rRm6OEisi2qG2Q7ZEPUA6IeEREX2RfbmhKf/SWW8xBbiWb7vDXqANVtSjs5FJQXyhOdozz2mQ7RqKvKO+mgSoXyGNjjPWxZuzqKrlh/pm1oW1J73rGkRryJ0zQKFjV+6euCEZbMnI6GsiX5n1yyS1G7vW5tVkVoxbE35pGkb2sXCJ3nbVoU07DYqGifujpkREyk0/dDdBomuzVVkC1ffvR7tyuOHz7o6TKcnvAbuk6rpGITZ6SsBNeu615J25rF0oXHGb1SUz4jdRn6sHvbFlnGb6NQseIQeUtA1JU9kYELZYN6zm8X1NMib43siG0nJlI07c22w+QWOz1cB8x6QFsKQ3mkvFKeyVNeIHXwiezy1+uodBMmddSv29vY/fsWT380fc18Um9LUiPexEkwE6oHtbWXuWrRApQrVgwiXymIHmMlWV7RHiUphRTFrZFZ+LaSzWPmNi2InY9AYhMptcbUmJCOxm9T47uPZs+D3p074eLFi0qvZCzJYRTJCequX7uuPTDlcaYD4iSYdcOco/rTwvloVLMGbs8pPcsGkhhGrdfeI5EGESaPHa70be06Yot5PFiY5Ba7zprpt9Nr1wXaknfKjQfp45tfIep3VDprWKsG1ixe6OnTHgpMLsSbONmomTTVeJxE9zdeg8iSA6L3JO1JUdeCuhSBPEpTeYEKPZCyEyrm/WIS+7rEFPteJGw8JNQCk9HMPwpRpSXyZsuBX39eq/Qb4fNakrN1TVRQmnnrE5qmRUgvxGnWDa7YB/bvQ5NqVSGy5oPo+rWsD9e0M0FDOFT+wepHMHsxw9hhbXtLCYkpbTHlgY8xkdI+6Yh0RToj3b2QH01lT/bQwQNKt+xQJGedSBBxMmnymEOr+nUhClSTRHlVE6bKPPytpq1cJ9GF9ET6Io+DPBFpG6LfNDzw0CNYt3KF0n1EeNQxnrQMTZwRikjb1ymJghVaqf20TpwkPJa5cM4sZMr8FMSrfbU3RT0LKmubLE0bMLc3iwTKN9cHIlH6TbrbIKV1Lzz536ex+Me5Ssf20/ekRoKJk1vU778aBpG9uM4YGceqG4FbFftYcol9by4Qm9yTK332vez7cvpWhUMcgppiki1bNlyS3Xb28JPLSJISlA8e43ypTC48l6+e8khpAjz3YtIaNGnqecprly9DhoyPQIxeC3HQV6bsSQUTLn/bZpJKzHva+8mZLvt+gYR1R7oc8RPuePBRrF+zhtpa0GvRyVUnEkScprdJb9eoaRL0pMw2DFaKrajkEjMNXgFAEzyNKfLTfDqWWtJMwmmhB2rUVSlcBctmTlf6N+d7pmWoxpfGxuV2zoRv8NWYGdrjjEy7XjWP99ODrxJFi0D0makr+orwlLcpW4LZOjsVdt2xr09u4XSQLsmh+GAqShUpquxFcZHPoUhqJIg4dcuqDbx8mdL6jR0a17THamylp0QBmAZA6SPCXHIGYvA8bdhTT/gfXtlDC8mVXttIWVZG6An3xWtj8vChSt96DmDaJ04iSZUHIx8qX75tWgR7ysf27cW/cxTUZWp3yQPVi5QSM0205XF2qg/kXKREXSAJpCtTqJ5KyZCzEI4f2OfpPjnsJsHEyW+AtG3SCKJsE4jDMtLVvqdjgYzDLqTkEDMdlC4i915DITL+L4QQSm4R/8YtdXr7DcUkz6RMq20MttCroeQRT98HcdvdmDRjptJ3Sj1NTGz4CVI/febulkeoaRBcec+ePInHc+bTr/zSeLVdJ1hsm0hOsdNCQ2w0Nej75RDl6svtCU2iNvEnh5j3s9NJ6aG6uuQi7sn8FI7IRsp05JIa8SZONg6uwO3atdck9PJH/vlnVAhEQIGUYCspKcVUNrWinw5Xab31oZwQHT6H6Pk1RMnS6ti99YfqqRC2oSRnmulePO5KnuYPO3DLk89DtOqFCavWKX3TW1npxePUW+8/3+G0my9dLrKOyP3cT/9Xvw9OXXUqW3s4KKXFJCOyN6q7QydJp+JOXZ+H79NPte36kJxi3pPrBu3Tg9MarXHXC3lw5PhJpfc043F6xNm0IUSDLhAVGkAULAsx7Q89LkeT3EnpwVpbW2xFJVQ4LjKKX6SsPgXx2AP490PSE5h3xf8GA80CqFxWep73Q0w8o8c8Kc1mHHEVO2+hhO7HTw8pPZSGLpLYH30Kou94iAUXMXHOPKVv1ntyGElSgz1O2piNQVrNm+dUyN5Y9nzSzh6T5VfjZekhRWpiUuUcoPxDiW1fiSEUL49nbpXSvb+/F3Z7Rojv/tJOkPnw1I4jIWLnkeO3j/FDNUon6fDXCL3QT/HqEF+vxNFdO6LoPqkRb+IkUCI94mxYF6J1X4iTMtL2AyCezQ1Rp41U/Gbt5RERcHcluaco0X3onuRJDvhSe5vvLNczAJZfg1hxTXffxy9Q5+5pM1mnN9BDrsQSjpPJkvYpDdTY0AsD3b+ByFlYL+Iw+Q89ED5hLyb8uEAZBs19ZJJxkrokCnFmzwExcgPEi6/qOtF/qnYmvPnN1rCWaR+2zSSFmHZI9YPmmuatIevuS7Iu/Ati1GG/82OnMamE78OEzg9y+b33/pO1Lmu20mkeuQZ/7dzucRLpPqkRb+JkI2HibN+4PkSznrriU2aWnJVE+iFE9oIQRatAdJPd4znH/e+qkvvPE+PNV8lMogolrGRb8YEKge5DLVWDmrj1lkeltxmp30rwDTDr8ZJDELcJ3Fe6a/DZAbaY6bD3bfEMIVLvU0PCi5nQdSN+gqjfQeusTG2IIQu1vshYKOzUg5g0f5HSNy184JC6QfUjZ7ZsEN9shPhLlt/n8/SSbvlLy4ZxNMSyC7oukK2R/Zm2YdtOKNuyxbbTYGJfQ9PfKE1vvaM9z++OJ4w445puVUd8fEA6Id1Qepae085EvpJah0MX6nPEI18tx187/lA6T5vE2eJD3fUlL44n+q6WBdF3su7Cv5BfutayRXv9E4jRG/Q0GyYOIggiB1KySaaBCNUulGASpTCgPd9COXH7g/mjrhZDQr9/loXz8G14KGsbHZYKMNB9bbHPmQTJ+aD4yQB5aTrap4ak3zTpjbSV3mUR/c5yqx7Ss9zpX+GGdUDkKYlz3Ky5asX0i5cu4cqVK0rot5PUI2a5FMwvu+ojftUVnJdk+/xHiJK1dH2o3hLi4xkQiy/46wI7FZ4N+fZjssVAx2IrfC1t2cFo3hi3iDukzV3QQ0bx6aqb8ZrHWMx6wnWEe11k/0tkfRwwXeqouX7jqmw93fiQfqhuEMnT8MKQJdiz6TfFQ6meOAlRuuqNZKaa+YiTW00er+MxRFoRqM8PshsgiSJ/Kb2wauk6EK98JBUyXy8pR8qzV4Ih0rCfdgcqlEDCYalgqFDyZMdtjxWIHp8iUllQD92Kh3K95ntAFAviNNPBwgRJBsDe9dJLEOO2yFZzLETddpLAy2rPsphsSNr1hRi7UXvAdA0/xTQrC51bcAYZ//s0MmXKJCWzb+skNUuGDBmk17Zd2wQ5ClSmZFtk4zMOQHQcBBFWSZNoqZp6mGvkWogVlzU5kD3wsopss8qx8NmH6ViYdmjbaTAx7ZbjprTVLIfbxX8h5sPfM4tr3Bw/b80GgPJCeTIdJ8oz5Z10UKKmJssiUjcdP4OYeVCHpXBMuLSlOj14MXZvXO9xUnIg3sRpe5xdWrXQHuc+UpCPMFnRXMBUAOx6k+Km7ob4cDxEvTek8VSEyFVUS5kXZZf1Tdm9HykJdbEMt0+v3s4T1u3CtgvLLDQzDaT0Uvnx/+7KpQuRwpDySSjupYch/iVwd/l3dSHZ3SY7fvseFJYeQH2/TXrZsqVs208a4MuSJCv48lYMolJjSZT9ZVdjufYyzKELNi6zIvA9KH2LJLH/5wFv8N5JGpEJf0R9wML1wVzYZfZhXRfoIRL1PGh8mxrVhp3k8QkQ326SdcDn/fEyjHwtkTKTaky2GkqYkFTPLBdufyCPP674xG1ewz1Qfui55KJ2JHpNhGjUSfdEKc95S6on5UoXs47qtNB1/HyEiZfi57SmJY/TJs732rXxd9V5bDCQEimzfJ6Xk+LuyfKrsnX+XXqlUyBelnFVa6nWqlSrLGXNK/fLQNRuF71Q7AKz70nCXZCm9XCrkOQzK9zfklKaVFd4tTL0e14aFfXhEMdlx2/ehysFEWe15mrld9H4LYhOgyGGrZAt5l/aq+WuOk8uVvrwbc17mGknIWObcgj9vhyBffv2YdfuPWrrJPXJ3r17ldB+3jy5Ib7+TXtGZiPMNsNlT+XLdYFIYuYhiE9mQbR6XzoSdXUPjbzSQrI+lJaORYv3IN76UjoWiyAm7tC9ObI9u9GNi3CaiIizZcFdmcv7u+mhHIhAwvGRnc8/LdPcXfcwyZGgvNBYL+WN8vjJbIjph3Te2cv2HiYHcWDoGOmViHOjnziTA/EmTgIlkifA93jtVYjmH/g9TrvisyJ5y0o1FWOOAxIBM8FQl3nOCdmd3aw9tUBxBxIzDN2DvN2+g7QX0G+z/r3iuh6TpcLq2k2f+/h3//hiXO7Bv9mzpjhJKC4yQDpnE2Wg6+190g0R+6QDGDN9llUKDqkZRQoXghj+q7YBKsdAdsNlbDoVZC9sQ1QfiHzmnpQkLLuyPb6VjkVP/Q0gIlEiIVpAxHygadtpTGKn47cbEM8/jvv/Wyf65Pe4xO3FB+09EkF2/1bngfJCeeLGgtLOdYTHPO2GxhYKY3XVaVHsG6nZ4yRQAtnjbF6jalSP01aiqUzemoXAvz0D8m2ZUEn55JVS19aOM5RwvERoi/dC3H0L7nqhjv+pNqV56UGIJx7E7ffklEQKf+ttxmHHa4uZNzsPZoEHusbet8OQUU3ah68nT1MN1jXfh63UavuR/m9cO0l5MVcpL1yggP4WDxNnsDK2yztQPSB75aUauavOniy9YRbMxmIrnD6KO/fzuDNzWU1o5rm4xM3h6VoiSYqX0k5pprpI54grAtUTM45A8dKWwrPHaXTVkwPxJk4yCjYS2i+ZK4dsBftB7EbMHmcgxcRWSFE2CZlx2WKHowIi4u3wlvIsb8leSXqZIyE6D4B4MpP2NjvN0mEobKj47fvY94zpXKjjplCeqeWf/Q+GfDdB6ducx+mQusCV9+zpM3g8U2aIcdv001/bc4tJgtkFxcGeKXtmph2a29gKx03OwkrZA1u4X3anc+PWB7NDTN2v3wA0w9nXBxM77aYnGaoem8cChaMtxUV6HbwQuzb8qupCqvc4udKyx1mrRDGIl6QrfhT6U7u2MmzFJKdwWriwNsjWudkrUQfw73oYos3wpH1LIpTY+mLj+PmG9rY/mIwZCxcrfaebVy7TIXgdzqNHj+L+++6DmPhn1Ck9drnHV0wy4XjjGj/XC+72Vigb7eHWXfU+9z8sjWv8iSGB6gUJedk0NDhsJbb/vNrTfXLUiUQhTtrOnjYVtz8kvbZpu3TXl99Ttws10L6tnMQWU9mcJjKS2fshPp4O8eliiB/PR+1O2elLLAmU/0D7JKRDGqagt4ZqtEOTqhWV7pN70VaHuIE8TlU3ZDk994SsE72m6TLkZeVsCWQDts3EVgJdH5PwdWT3RO4ffqkfbtKDWZpjWqUVxKDfoo512nEEEzttwcS+Jti+KUyany9Enpfa4cKZ096wlawVdpEkOuJNnAzlHvta2EljRyHDw5khBs7R5EkPengMw864rQxbUUkpdB8yAh6AJ+FvvdiFZV+bGGLn2RQyYB4jpjGsNZfU55PLFyqAc+fOya6If3jEEWfqhFcn5HbOD9NwT+anIb75WS/0QWUcl6Gs5BS6Lw1T0TikKTxVzg6f2BJKJ1w3aEu6HLUW9z2SGRuXLlR6p4/hqfqQDFUi3sTpVVzffni4frq+evlyFMiZG6J0XYjxW7XieYDZfppsK81WZGKJXSBmQfCYEe3b4e14EkPMeAOlg8icBtHXyha16wjc+mwOvNO2Da74hkTYm3GkmXrBZcMOxbJ5P+KxTE/q+bv0gIfqBJU/fR7F7pUlt5j1gX6r8VNLuG4kdzrN+sHOBOmOnJxX++JxqdPli+YrHZsLeydHvYg3cTLMhPJ459Vr1/BFrw+RNXde9dF50W8G1KuXPO3Am78Yy2kHtphKjWtBBgsfKL5gYe3wcRUmSvY8yDPnaUtT9kK07Il/5SiIRrVqYOOaNZ6e+aGDaRjJYSQOcQO5E1QsVDY8XW//vr1oXL0qRI7CEN1GSxu4rntl3osPvrpg20psxLbN+EiweMzjwcIEEzudcRGzfpCO1Gyda1J3o6QOi6Bp9erqA3iE5CZNQoKJk/1iu5UlXL5wEZO+Ho5qFSrglhwFIGq0hfhkHsSiC/7XKqlrwBPRgxFpoAKxj6U24bR7JOnb0jlzrirlffx2iNc+hQirjOfyF8Dbr76C39f/4umRDMMmzeQyEId4QBVNdIeCsGrBPNSrWkVWflkfGnWJ/nngQC9FBKsH6UGi1BFf/ScdkC64fpCOGnbGbbkKo2GNali7ZJGnz5QgTUKCiZO76raYxkLYtXUzhvXthWoVK+KhPIUgiteUnlVviC+Wq3ew1eAzKcqcDOstdGAolYnVJlfbwExDs48npnBauOBNz4EKnZcQo7zRlsJO2A3x/veyIXkVIm8x5MhfEB1btcCS6dNw5dJFT2f0cMEez0xuA3GIO0yPkyUyQo9NM3Zu3og+XTujYOEwiNxF1XfDxcD5EAvP+UmDGleqB2S3nn359mOyf1tsu7d/B5Ng4cz6ZZ+zw5hiOhFcTygcv0HIjQfpgHRBOslXFAXDwtC/29vYtWWLpz+qGzxsRbC3SY0EE2dMoIwRgdIDDRN/HzmEhVMnoUeHdihfpgweISItXAGi6ssQbwyVSlsCMf2wXhSB31UlpdKWFEuuO7XSvMIRF4hNXokhXMi2wVKBU8tI6aCC51Vd+E0POr5AGsDYbZIkJ0I0ex+i5IuykoQhV+GiaFavLr4e0Bfb1q1FhK87x6A5mqZRMOzfDqkfZpmxQ2ESKDHspp9W4pP3uqFK+XK6LhSrBtG0O0TvGRBTD2ib4zds+G06diyiNNq+fbbT2JCr7f0F+21fZ4vpQJj1htPCBGnWZ3KQ6Ny0gzqvTd5V7+c/nKeg0gXpZMva1UpHDNOZ8KsweUmTkKjEabawtlBmabwnitH4cPbkcWxYsRRjvxgoPa+WqFq5Ep7NWwD/U6AERP5yEFVaS0PqAdFphCTVxdJ13wQx8xjEsmv+1zT59bR9PmESo0LixQVYyAhJ+Ik67Zvn+Rq6ngqY4mOPkY2WDGLJZf0qKC3qMXgpRJsBEHXfgihRB6JAaTyatzAKliiJJi/WwoB3u2L2d99i3x+/I9L3II1BxU26sbsdtqiwyWgcDgmDWWa2cH2wy/P0sb+wYtZ0DHjvHdSV3dLnCxTCLfmKyR5aLYgGb0N0HgkxZBXElP16Sg6//cY2yrZOdkr1gs5Tz0ctmwg/yZkEZ2490jP3rd4eEyrFSXHTPehedE+uP1xv2GumtFKaKe1dvtZ5oV6n9Cgpj/VkXqmOLJ81A/8cOxpFJ6Qj5g5bjylVLxKVOG0EMxx+4yiQN8q4fukyjuzZhTULfsS0USPQu0tHvNG6BWpUqYKChQoja/HSuDev7ObkJG+1IkT5JmrajiLZup0gXvlYe69kaO+Ok17fJO35UcvWfw5Ev7kQgxZJIl4I0Vf+/mAKRI+JWrqM0iTdfpB+/756W4hKLfQ9aFEC2bX6V86CuDtbXm+S8MP334O3X2mNcYMHYd7E77FVtpTn/j5pZ8sD5Z29j5Q2Aofkh1nWXl2gzyRbCL96Bfu2/44l06fisw+64/VWzVGuTGlkL1wMd8vei1r8gz4hQXbf6iOIt0dDfDQdYthaiAm7IGaf0F1fIi4iOiIxIjhyGGwnIdA+OxnsaJjeLsVJH6Kje9C96J50b0pDqz46TWUbqcU87soThmxhRVG2dGmVB8rLkulTsG/bVpVHG8QLgepHaqkjSUqcBDuD6jf9s5TBxhPMK7Vx/fJFnDx0EHt+34zNP63AqjkzMGn4UIz6pD+G9PoAH/fohvc7vYGu7V5Fo1rV0bh2DdSvVgW1K5ZHnUoVUL1cWYTlyoli+fOq3yQUhqR1w/ro3vENfPR2Zwzq2QNf9e2FsZ8PxNzxY/HL4vnY/staHPpzB47s3Y03X+vgkWfNOi9i38FDUdJJK7VTntijNA0BSjcB9OOQbuGv/H4SIDvg4zy8xRIMEdeu4tj+vWqoZ8Hk8Rj5cT9l8+2aNcWLVSqhYqVKKFS6HDLnzIuHs+fBf/JLr7VoJdkVriJ7Q+UhCsn9sCra6ysmpWgNPVRWr6N2PCq3lD2n2tJhkM5CxcbSOamqr6Fri8vrilVWcT6cPa+6B92L7vli5YoqDZ+89y6++aSfShulkdJKaQ4GM89Rp9zZIVNHHUlS4uQMmgSpfivmjE6etpieqUk8ifHZiCN//YVHHnsMJUqWsk/FGfMWLEDWrFkVed5xxx3o16+fR/7Xrl2Llq+oEkA/iZA/h9QNLuJANmDaAQmTqVcHgvTSouMGrpw/h7MnT+DEwf04uH0bdv72i+rFrZSOxoLJE7Bg0njMlz0k6tVNHTlcyeQRX2L6mJF4l1Y8kzadUcoPo0fip7mzsHreXPwp46C4ThzYr+Kme8QW7EkGciQCiq+BCaSblESSEmdig1RlKpAMismVCTaUEJFR2K1btyqjyPp8VvWbHshwYcYkirgj/Cvh8Dy90/+cxquvakMjqSRb3x07dqhznD6z4B0cYkJQO7Hsn4koipCNktdmXxtHLFv1E8S//41HHn8c1yNjjo3OKoIPUA+5rgSz/0DHUjtSFXHaLUtSCBUkbf/44w9FcDly5FC/7cKNizB5EiZPmoynnnpKt9QZM+LLL7/0zvHDAFMIvHVwIJh2kRjCts1iE5st9L0k2s6ePVuP3z/8ME6ePKkdDN9Qmhmffb/AEjw/dp7TAlIVcSYHuAttEifBLMS4gq5jgyQcOXIEjRo28rzPWrVqqRXBCdwC24YS33s7OMQFsbEztuMff/xR2e8jjzyielQEsl0HR5zxJk671WRhoyOMHTtWfbCL7vPYY49h9OjR3jn2fAnmfeOSBgeH+MC2WVtCEaezUUec8SZOEzYBmt7nvn37UKdOHc/7bNCgAQ4fPqzOmd5nQu7v4JCYCEWcDo44E4U4CSZ5sphjnyNGjFBjRXTPJ554AhMnTvTOsffpCNQhNcARZ2g44kwE4gxEmiz8RJ2wc+dOVKlSxfM+mzdvjlOnTqlzJnnacZn3cHBIajjiDA1HnIlAnDGB4zW9z0GDBuH+++9X96f5n2SgDHPsk2H/dnBISjjiDA1HnElAnKa3aIrpfW7cuBGlSpXyvM8333gT58+fV+fsaUscp4NDcsARZ2g44kwC4owJdA/2PskIe/furd42orTkypULS5cu9cLZY5983MEhKeGIMzQccSYhcZqkZwtPIiasW7cOYWFhnvfZtWtXXL58WZ3jJ+8cnxm3g0NSwBFnaDjiTELiDATTcyQjZO+TXgXt3r07brvtNpWuAgUKYPXq1V5YNmaTfM34HBwSC444Q8MRZzITJ8EkPyZFvvfKlSuRL18+lbbbb78dPXv29MiVxz7NeBwcEhuOOEPDEWcyE6dNfKbnyAR56dIldOrUyeu6Fy9eXD1MIvADJpt8kyPtDjcHHHGGhiPOZCZORqB70THT+5w7d65avYnSee+99+KTTz7xwppP3hmB4nRwiCsccYaGI84UIk6C7THy/c2xTzLYV155xfM+K1SogG3btqlztvcZKE4Hh7jCEWdoOOJMQeIMBk4LGzDhhx9+QJYsWVSaabm6oUOHeucCeZ8ODvGFI87QcMSZiojT9hZJyFDZkI8fP45mzZp53me1atWwe/dudY6nLdnXc55SOm8OaQeOOEPDEWcqIs5A4HSZ3uf48eOROXNmz6hHjhzpnbPfOkqNeXJI3XDEGRqOOFMhcZpeoimm93no0CG1RB17n/Xq1YtxubrUkjeH1A9HnKHhiDMVEqeJQCRqep/kbfJydfTJjimTp3jnzCf0NoGmxrw6pA444gwNR5ypnDgJZro4nab3+eeff6Jq1aqe99myZUucOHFCnaOuOxu7I02H2MARZ2g44kwDxEkwPUZTzOXqvvjiC2+5uueeew6zZs3yzrH3aQohNefZIWXgiDM0HHGmEeIMBkozz+ck/P777yhfvrznfbZv3x5nTp9R59j7tMnTwcGEI87QcMSZxogzmNdIYi5XN2DAANx9991eHhcuXOjFEWzs0zzmcPPCEWdoOOJMY8RpwyZAyh8b92+//abec2fvk95/v3DhgjpnT1uy43O4eeGIMzQccaZx4iTY5EliLlf3wQcfeORJKy/FtFydGZ/DzQlHnKHhiDMdECfBJE0Wfo+dsHz5cuTNm1flmdb87NmzpyJVgu19OgK9ueGIMzQccaYT4gwEzhN7n/RNozfffNPzPosVK4b169erc1QhTKJNb7pwiD0ccYaGI850RJy212iSoDlpfv78+ciePbvKPz1A6tevn6cXe96nHU9ywM6DLTDS5JD4cMQZGo440xFxBoJJemT05nJ1bdu29bzPsmXLYuvWreqc7X3a8aQ0Uj4F6RuOOEPDEWc6Jk6T7Ewxvc8ZM2bg2WefVbq477778Nlnn3nn7OXqAhFpYuIGUaJyKP1pZRI3558iSJocEgeOOEPDEWc6Jk4bZv6oAnAFoeXqmjdv7nmfVapUUa9xEigMe5+mJAmse5gETwinYQQzjO8ah8SFI87QcMR5kxCn6aEFI6eJEyfiiSee8CrLmDFjvHMUzr42saGJUKeRK+i1i2exffs2HD91zh/uhvY+iTQTPxUOjjhDwxHnTUKcBJs8eZ90wpXlwIEDqFu3rud9NmrYKMpydaw/m0ATQ3ccX0Skvse4L95Hpvvv0mm57W40af0uzlyiJfPoFdNIRZqJcV+HqHDEGRqOOG8i4rRhkh+JuWAILVdHFYZ09OSTTypvlMHeJyOx9HbDN3xAsS2Z9Jm6d/1WnTFj5nQMHtAdd8rfDV7TH6wLD/eNv0Ymzr0d/HDEGRqOOG9S4rSJz/P2DO9z165d6vMc7H22aNFCjYcSKIz5wMaU+IJIMDw8ghKE3m3ronj1NlHOf9W1Ie59sCCu0z0i/fd3SFw44gwNR5w3KXEyOM82kZre55AhQ9QH4khfzzzzjHoSz+Bw9vXxQaRv7JLGMCWL4lqEf2jg3N+HUaXgcyhTvzPxKiLC+UGRq8iJDUecoeGI8yYnToJJnqaQrlhf9Eli+jQxe580B5Qrk71cnR1nbKHCkjMpyZDGMQkXTu1CuSK58X933Ipn8lfC3+HEqTJd6n60H/v4HWIHR5yh4YjTEWdQsE7MJ++ffPIJ7r33XqW7F154wVuujsPZ5MuIlW4Vb/pIW3qU9ADo7JE/0eudt/Haqy2RJdNjaN6+F64RuUbqKVJEsg6JC0ecoeGI0xFnNNjkx6TIuqPl6ooUKeJ5n507d8alS5fUOfOVTTuuUNBBLML19oA1PwxR9/t+yXZ15np41LebHBIHjjhDwxGnI84YwTph/ZjL1fXo0UOttER6pOXqVq5c6YU1vU87rmAwSfbQ/j04cOgEIuT9Ll+6iCtXr8rjp/DU7bfhjV6TfGkJd131JIAjztBwxOmIMyhM0jSPkQ752KpVqxRpki6JRLt37x5lubpgY5+BQGHDI+h8JKoWeAp5SrWMcv7Y9sXqPoOnrFO/1ZP9GOJziB8ccYaGI05HnDHCJDxTqAKx93n58mV06dLF67qHhYVh3TpNbhSOiTZYXAx6Qs7zOGeP7qPiavVGLyxdugRTxo1E1icz4r/ZyuHUFXp3ncSVWVLAEWdoOOJ0xBkvsL64S05YvHix0ifp9c4770Tv3r29ihao625DE6EmWllSGDmgKx66+1Y/IZetg237qQIbZOy66okOR5yh4YjTEWecYHuLJKb3ee7cObz++use2ZUqVQqbNm1S53h6k309613t81N1XwW9dPYkfv31V+zaf0QngOIxuv+uzBIfjjhDwxGnI85Egzltac6cOerb7qRj+tb74MGDvXP2pzr8oN++PSJPX1l5Z1X3XFdcHc6VV1LAEWdoOOJ0xBkvmMRninrA46t4J06cUK9psvdJr2/Sa5wEKgfzwZEWFXPUY74xUtvL5PC0dUhcOOIMDUecjjgTDFN3vG96n7RACC0UwpWQFhBhmGOk0YnRJ/qkEvM4X+OQuHDEGRqOOB1xJhpsAjS9T1qarlGjRp73SUvX7d+/X51T04pchUw1cMQZGo44HXEmKqJ5izeiLhjy7bff4rHHHlO6z5w5M7777jvvnPnkPZA4JA/iRJy+4ZKgQkFU2aWv8nPE6YgzWcDzOQn79u1D7dq1Pe+zcePGOHr0qDoXaNK8Q/IiLsSpqdH/S/0foNyiH0nbcMRpEScXerSW0xIzjENU2LoyxfQ+hw8fjgcffFCVQ5YsWTB16lTvXCDv0yF5EDvipDJRG3/53NDnVUndiMR1KkPaVefSV/k54nQeZ5KD9co6Nr3PHTt2oFy5cp73+eqrr+LUqVPqnO19OhJNHsSGOHURRC2T8OvXoM5GnEXV4mGYsHCLIs5wmrubznxOR5wBiDNoxSTj8J4Cu0ocV9jkx94nVcZ+/frhP//5jyqT7NmzY/78+d515pN3hv3bIfEQG+KkukBFwGXpHb9+Fi/X0CtnTVqyTR8KUH5pHY44LeJkoxk9oAvuv/8BZHwwIx544AE1ifvBRx5TK6BXrdZRh49IfwaRVGA9mVsSns9JoOXqihcv7nmfb77xJs6fP6/O2d6nGYdD4iI2xEkepFeG9GKC7Jr/PG8i8j7zBP773+dx/53/h/ELDI8znZWTI06LOOk8bVf9OBnt27dDhw4d0K5dW7Rr+xpebqIfaLR772sdl5pGk74MIjlgVyKqjOZydT179vSWq8uTJw+WLVumztF15tinQ9IgVsRJZQDfsAutMRB5EbUKZsVL7Xrh76N/IONtGTB23hYdnyPOtI9QxKn2g5DhG3VLIk+JJqAqHhGhlzRLbwaR1PB07NOdKWaXfM2aNShYsKDnfb7zzjtqFSaC7X0GEof4IzbEGeWhEP2MuIYz/5xV+1ePbca94laMW7BV/XbEmQ4QijiVRFJLGqG8H6qs4RGRmDm8B24Rd2HjgXNkJT5jiO49OcQdXuW7EXXBEFpVvmvXrh55FipUSBEqgR8wOf0nPmJDnDeMhz1mGVAv7MKRzfi/f92GsfP9Hmd6gyPOQB6nb1+v+RiJ8ItHkOU+gdZv61cFgy9S4RAfRGm0fMKVl0BddeqyU3llyJABffr08SqwXRauXBKO2BCn8ji9cU5fo+frMVw6ugn/K26THqfrqqcbxIU4w+nTDPL3+E874NYMT+DQOfr6Ink6kd7K4+nLHFIXqAzM5erat2/veZ80hWnrVt0VpDJl79MuR4e4I1bEGQC0ehXh0tEtyuM0u+rpDY44gxGnb9AbEReQ78n7UKv1AHUuPDy6h+OQOLD1ymJ6n7Nnz8bzzz+vyo5mOnz++efeOfY+Ca5c4g9HnKHhiDMIcdJYDWH3umky3L8wb/1RXyXWk3ldBU16mORpjn2ePHkSrVq18rzPKlWqYOfOneocT2+yydcUh5iRGMSZMcOd+G6hI850g1gRJ41r+ryXr7q3xIOP5sfFSArj/86N+0hY8sFspEzvc8rkKXjqqadUOT700EPqFU6G6X06xA3xJc7ICH3dxcO/qetGztmoftME+PQGR5wBiJNI8Xq4fue2adlcCKv5BoWQXqh7KJSciNKYmeVDDZuvMh45cgRNmjTxvM86depg37596hyFCeR9OsSM+BInfS+KcP3iKYz8ajj+PKRfnY3pmrQKR5yBiJPGN28QVV7CU/cJNHltqDp3/ZqeP+iQvDDJziRS0/scN24cMmXKpMqUlq2j5esY9pN3giPQ4Igvcd5McMQZgDip5bxBb0XcuIRRX36OVev2KO8zMsCTW4fkg0l6LKb3uX///ijL1TVt2hTHjh1T52zvM1CcDhrxJU5TlzTUxW/VpUfdOuIMQJy+A1GmGqW/ok/7MCuquVzdsGHDoixXN23qNO8ck4JZ3vb2Zkd8ifNmgiPOAMSp9vUOaPoRf6o2Grk6pBi4LEzh+ZyE7du3o2LFip732aZNG2+5OvY+OR5z6+CIMzZwxBmEOKMcM4jUIfXAJjsuL9P7/PTTT3Hfffepsqb5n3PnzvXOEUHY5GvHeTPCEWdoOOIMQJwOaQuBGj3T+9y0aRNKlizpeZ+vv/56jMvV3exwxBkajjgdcaZ5mKRpC3ufVO69e/fGnXfeqco9Z86cWLx4sXedPfYZiIxvFjjiDA1HnI440x1sIuW3iQjr1q1DWFiY533ScnW0ChaBvU8TN6NNOOIMDUecjjjTJWzPkYS9z6tXr0ZZro6I9Oeff1bn1BxeY9oZx2Vu0zsccYaGI05HnDcVmBQJS5cuRe7cuZUd3HHHHfjoo488+7DHPm8m23DEGRqOOB1xplvYHqcp7H3SQyL6thF7n/QQaePGjeqc6X1yfDcDHHGGhiNOR5w3Dcwypi0TBGHevHnImjWrsgmavkTTmBj2K5t2POkNjjhDwxGnI86bCjb5ERGw90nkQN91Z++zQoUK2LZNf+LWnN6U3uGIMzQccTrivKlge4pc7qb3OX36dDz99NPKPujVzaFDh3rngnmf6cl2HHGGhiNOR5w3LezyJlJg0jh+/Dheeuklz/usVasW9u7dq85RGLaj9Gg3jjhDwxGnI86bEra3aIrpfY4ZMwaPP/64shVatm7s2LHeOQrHNpOebMcRZ2g44nTE6WCA7YCfqBPI0zSXq2vUsBEOHz6szhHJ2NOWTEmLcMQZGo44HXE6+GCTHom5YMhXX33lLVdHn+yYNHGSd84Ml9bhiDM0HHE64nQIAdP7pI/CVa5c2fM+W7RoocZDCenF+3TEGRqOOB1xOlgIRHjmtCXCoEGD8MADDygbeu655zBz5kzvHBNPWrUpR5yh4YjTEadDCJi2QfbD5LFlyxaULVvW8z7btm2Lc+fOqXP84CgmSa1wxBkajjgdcTrEEibp2cvVZciQQdlT3rx5sXz5ci+8SaBpBY44Q8MRpyNOh1jAtA3eN73PtWvXonDhwp73ScvVXb58WZ1La5PmHXGGhiNOR5wOcQDbiLll7/PKlSvo1q0b/vWvfynbIiIlQuVwdvfdjCc1wRFnaDjidMTpEAfYHiML2RXbD3XVqctO9nX77bfjww8/9MiVvU+OKzXCEWdoOOJ0xOmQAJh2Yz55v3DhAjp27Oh13UuUKIENGzZ44Zhog0lKwhFnaDjidMTpkAgwSY+Jh7BgwQJkz55d2drdd9+NAQMGeORjep+pCY44Q8MRpyNOhwSC7cbcmt7nmdNn0K5dO8/7pOXqduzYoc6RPQbzPlMKjjhDwxGnI06HRIRpQ7b3OW3qNGTJkkXZXcaMGTFs2DDvnP3kPSXhiDM0HHE64nRIJJgepylENkxGR48eRZMmTTzv01yujqc32debtpkcNuqIMzQccTridEhCmIRnep/fffcdMmfO7BHTqFGjvHMULiUJyhFnaDjidMTpkEQwSZPF9D4PHDiAunXret5ngwYNoi1XZ1+fHDbqiDM0HHE64nRIBtjkZy4YMnLkSEVOZI9PPPEEJkyY4J0zxz4JyUGgjjhDwxGnI06HZIJNfmSLTFK7du1CtWrVPO+zWbNmOHHihDpH5Gl6n0kNR5yh4YjTEWeiQ+syupcVXZKHCFIDoufdT6Km9zl48OAoy9XNnj3bO0eEZseRECHw1tu/4R+LjUKcp06pYzE9vAol6QmOOB1xJg0sVabXCpQQmDrh+ZwEWq6uTJkynvfZoUOHKMvVMXmZ8djxsVBYEo6fheIJJvTOPW3nzJmj3rsn4jx58qR3Hcdp38tOT0z7aR2phjjtQogqwc/TSftYTMLdHvpeNhklvdVhGpYdPjHEzh//Tm/QebLyTw9Drvs/ahYRob8QGU1HtI0aXbqGnX8W9j7JJvv166feNuIGftGiRd71FM4kO/4dQYTmhUoYlqxYqe79wIMP4YrPC7VB9+L6YxMwHTMJVoUPkOeYJLUiVRCnqdRox3z7pphEZxqNEt9xr8C8GKNi1+49ELI1zZErl33KA9/bbxh+4/AM1b5fgAK3f6dnUFZv3CBd+LZ2AB9IZ3SSwhBUuJtIT4RgdsE2Rfj555+jLFfX6a23cFF6hDEiMgJXLl7A2ZMncGzfHhzY9js2rlyGnxfOw4pZ07FsxlTM/f5bKWMxx7fV+7T9FrPHjVG/B33QHcVyZkfFIoWxbOY0/LZ8CfZu2Yxje3fjzIljuHzhPCXWvntARETo2QTXr0cnVGUrQS0ldSLZiFMpxqcgT1kGyZhbJiuPpHwEFS/VSiMKv3IZ4bKQL535B8cO7MfJQwexZsliZYgvZHlK/T5x8ABOHj6IK+fO4trFc4i4ekVdG1dQ2om8TTK3STW4qBh827QHXYb+8ouUP69d+Bt93+mAbM8+gQel51KuWkMsXvOHCq+8I1/eKdNpNNtBofUR3c7Vn1HIfN7vtUW39YFffIGHHn5Y2WzeF7Ji7qTxWDh5AkYP+hgf9+iGVxo3QP3qVVGpajXkLFQE/82VF4/mKYi7i5aHKFoJIn9piHxlIQrL/cJVIMo2hqjdHqLGqxA12/qE9l/Vx2q1g6jSUkpTiEoybP5yEHlLQRSpiDuLVMAjMu4nc+ZFzsJFUFHek+7dukFd9Hu3K0YP/Bg/jh+H7b/8jGP79+q6FARUVwJ5p1HE4I7UgiQlTjPj0ZRhCCmNiIaEW9rAuIFLZ8/g0M4/sGHFUtVyTvxyCD7u1hXvv94OHV9pg6b16qFcqVIoUawYChcpinxlKiBrsVJ4tlR5PFGoGO56IRfuyFEAGV7IC/HfrBBZsuHO7PnlsfzqXJaipfFsyXLIVrwM8pUuh7DCYSgl4ypfuhRaNGyAt9q+gg86dsDA99/FtG9GYMXsGdj00wocly371QsX7ARHAVUI01O19RBI0hI4zdpDl57kjStoWaWQqux1GjRD23avIPtTD+FW8TAWb9gP8jYVSaTR/MYGKl+8tcQjygA2H3H9Gg5KO18yfSqGfPQB3nujA57MlAm3ZJTk+WgWiBzFIMrUh6jcGqJNf4guX0N89APE8F8gJuyCmHUMYuF5iJ9kJV8nZbOUbVJ2SvlTyq5YCIX93Sd07RbouCjORTLuWcchJu6GGLEeovcMiM4jZVoGSLJtrdMWVlHVtWxFS6JsqZJ4vVULVW+Wyjzt2/67yqMNdjoCESlSkZ0kKXF6MPKpDMbnTQYjyYun/8GerZswb8J3GN6/Dzo0b4q6tWoij2zdnsxXCP8uKFu+vCUhStaRreZrEPU6Q7TsDdHtW4g+MyE+k97klz9DjN4MMeUAxIy/IH48DTFPytJLECvDtawmI4iEWHFdivy95KIM94+W6UfktfsgRv4GMXQNxEAZZy9pHG+Phmj+AcSLnWTrLFvlYjUgchfHf8LK4qk8+VGoWAk0qlMLndu0xpjPPlFGQhXg2qWLdjYVbK80LYO76BHhepxu29KxijRH/bjZH+jqKeTLkhHl6ndTP8NpzFmRS+qoEIkJLz++Lf3mnpSdV+pWr5o7CwN7vIeGtWrghfwFcGs+SY5Fq0tb6wjxxpeSoCQpzpFktRaaCInY9krZ4yM6IkQiuU1SfpOyXsrP0OHX+IRsPj7C11NcFOev0Pege9E9KS2UBkoLpYnSshG6nk07CPHFCk3u9d6CKF5Ter/F8EKBwmhYuyY+fvcdLJ81A/8c+yuKTkhHUXps1rmURKISp9k62KJaV3pIYLnb5KX9/vNqjBs8CG9JsilfpjQyS3IUBSQ5lpDEWF+S4pvDIPrPg/huB8TcU7oAqfWjwqKC2ucTbiW3Q7eQbERUgGREJFTg1GqSkAHwPgmd43B0DV1L8ZDsgN84zPvRcTLiVTcgZspWfsxWSbCy5X/tc03qklhvyxOG5woWQeXy5dDj9faY+s1w7N68Sba4mmAYZCDc2tr6MyW1Qhu3JgfaP7Z3K4YMHY5z16mBuI7zl/SnJDo2LIUcRRtruwinh3W+fKXerMUKXDZ2eal8RvjHLRnbf12HLz7siVqVZNc3Vz7ZDa4M0agrxIfTICbt0YRF9kVERLZG9kxkRXbLJEgNP3mANsExyQUSJlNzy2L+NsMEk0CETGmiLZ2nekX1iOokkyrlic5N3q891cbvyLxXwUM58qJyubIY9H53bFnzk9fgEFh/Zh2wdczHkwOJSpw2uHWlrR831NgHeZKNpWf2bL6C0nssAVG1FcTrg3XLNPdvrXBqtbhFZYKyDUcVFImvsGJrPLERM54o9wtgrJReSptJ6JxuCjP9MMSniyBe+RiifBPppRZFtrz50aJuHYz9YiD2bttq6EgbiumZ+E0i+YwjXiD+s404MkIRZAQduv4Pnn/kTrTrNU6doocFnuGn9rzFAv5Krfdtwjywc4ckhh4oXrw4RJ6iunHtJ3tJ8//RDTTZDNkPkQ3ZOdu3ademjZqkl9zC9w+UjkD1yMwLhaE8stdMxLrgDMQn0kGq+4bSTcHChdG3a2fsMeqGyScpaS8JJ06jophCGTTx28rl6N6hHQoVld2PfCW0wXw4VXeJybtjoiEFEgGRwm2Sso0mtmIXZGwlkBHYv02JYiCUbh+Z0zWURyJRyiM1CES04/+E6PwNRMXmuC1XEZSQlWlgj+74c7O/a6t0GWFOfI5KTFEIKoWhyc+fvkjfeBV5m/Sg6AauoWWV/HjoqTAcv6ArAD8gYjtKy4haJlHrwG8/rUTzF+vg7lzSUajTQXa710D8Ar8HRr0ctpuE2HpMYpNbfMSOMyFi1hXaUo+PPWzSx8hfIBp2xm05C6Jhzer4ZdkST5+sW5NzktN+Ek6cPtb3Kot6MKBb2MuXLmHc0MEoTa1rXinN39fjjqQgbllNkrQNxi40Ljj7WGoW01A4jyT0mzxoHoinY0NXytb2ddyaqxDqVKmEORO+97SsvE9flzY1kqaCLzmmIRNpKlw/jWaV8uG+R7Jiwx56CyXqd3p8Vxr7aQ9R860r9l9/HUXrei9CZJeE2WkoxMqLmhio2002YHZr05ptx1cC5dOsI6QT2icdka5+ugbxjnQwchbBSzVq4OD+fUq33CNLU8Tpr7z+fbNL8u2QL5AtRy6IUtJoPl+kWxAiS+rKmgYT15bVVnhqEjutoYTyzp4pXc9dtdXXIT6aDBFWGcULh2H+tCmezr1uiqf/5DOWUCB/U/0pI6aHRJo8ju/egCI5MuHJ54th+5Gz6hhNMaOwenzTd23qyUq8wJWX68GaFSvw1BNPQTTrIXsYkbqBVE+lLbKMSWwbS0zh+O37BEuDHS4hYt/Dvh/33rhukO42SHm1PzI9mQVL5s9VOo7yPMAsjCRGvImToCqtL9FcoY+fOIZapUpKD7MMxJh1ultK3W9SCCnB9igTszDSkpj5Z1GG4jvOD7k+nwfxXD60f6kRrl7XK+VERgSfbJ+S8Kcp0iPNnesX4MkHMqBEnZdxznr5JFI9hadpS758pKK8xAdMmrT9ff16ZHwsM8TgxRAHfWW60ip3c5sSYhNWIOFwZng7nsSWQPenh6+0PSBl1Frc/XAm/PbTKkWWqqFK5roQb+LkRGpj0e+uHj96FHmzvQDR8gP/U0Aen7SVwQqxlXYzSTBd8NgPCY350CB62aaoWrI4rl27psYFzTluqQX0eiUhMpLm4N3A1X8OIlumB1C8WhucuxqOC+fP4PTpszh//px6wq6mL/nyoaYkpaK8xAc0lMLeZvXy5SC6jII4Cj3dze5Z2WWe0kJp8XpA8NdZ87x9TXKIqTNKH01vooboo+koWayoGkcne0vuupAg4mRhY2lUqwZEk/chjiFwd8RWBP+2lZXYEtO91ZiKbxssXFKIfS9z3xRKG20PSQmrhb7duipdm+M7qQWcnuvX9LjmsG7N1TzOWzPc5b0yyJKrSlO6ApHhqedbOwkF97rOnjiO/6OHQCsjottSoN+2bSSl2PZFwg01DaNRD5Hng3J32U67HWdSiJ1GO70yzO25Cqs3k1j3N5Kxs55g4mTSPLJnF26TGdEZvxFV4aYybAUll9j3JkOhLY0rsqHQb1+hRAufEsKGQmklr3PKfmTNlUfN/1ReGj+RTiUgLxPKJnS6ls+egoGffooB/fqhb99+atGK/v37q+2oKXOgPc708+YQlwe9J/5cgUIQM4/r2RSp9eGPQUKKNMeshXi5O0TzjyC+XK/TTk/+A3nLySnmvUmX9LxkyUXc8/iTOLxvj8dDyWk/iUKctN23bQtuKVTGP63CzKyp7JRSPG9ZFFFKt3/oQogeYyFGb9XHiaBSijwDpZOE0kOzD+acxNM5c+Oq7y0kRZ6+RTJSA7RN+G0jFPyE6du3A6QxcH6oRPI8/RRE6Xp6TI7Ih8foQolpB0khtl1Rff3tIkS9htF6BbeWaud/OGOSpx1nQsTMcyA9mMK9L9Jp9Va467mcOHL8pNa5arTsEkk6JJg4uZWlFVuyPP4oRP85OmMrA4zrJLbSYyt8b+560NO5r2dCZH02iqH869mSEBN880rtMZ7kEttoKL2rIvRYWZs+yJI5Ey5cpvFBn/5TEXFqaK+TQI0qr0FgS9T3s30Wn5yWnwTgrnr49WvIkS8/RKZnICo1g1h3TT/so7IM9IDULnfbJhJTTLtSx/6BKJhX14HyHaTXuQFi5GKIYkXVsbtfmaLH2Zm0kiJ9dpx8HxYeSqN0bJQNUKUmesGS4Svw1+4/Pd0nZ9ObKMRJoIVWn3lGEtEDkjy/WqrJk5RAmTa77bZSbMXZSkwMiWYspyD+I4lS3C+7Jh9DfDwJ4pW3taE8VVXPM01sI7HzGEz4vCLMG9ojIF2+/SXEs/kgun+JC0cPRymD1AKVFsPjtMUfxj7H11gRpjFwfqhhKFRQdtUn/gnR6HWIp3NA9J2o5+3yyw/2MwDTRmxbsfcDXRNb4Wu5FzN/F0SWrBDtJ2hypy47PdhdIo/f+m88WOxdH3EahG/HGUzssGa67TB8joWfPfAbhNRD/FQ6O89Jkq/YSA+xjVyDIzv0W0XJPWyVIOIkMHGeOXkcWUvKVqDfDIgnJIE27yYVck0XAhGROW8zJsUFUnhCxS6U9TItPUdDfLNHT7Alg5ZcJN7tqVvecWd1l53HQRMq9v2DCelGGUykNmpK25K/Iao205Vv0VGIYQtxaf8erwyS01gcYobXCMj90iWKQ3y3XfcUhiyUXp2sGwVKQ3QbCbH8gq4XNN2MVxuyiZQlNvZj21tMYsfL6zQQYZL9LZd1VnKRGL9Y1YWM9b7ShMqT0mNzP/sewX6bwnZPQukh3ZCOVlyC6PEtRKGy+tXsz+ZqIqfzXyzCnk2/Kd0rHkrGqhBv4iSYFffsqb+RNX8hrfyl0qOr0EC2DrkhOn4uf5/VSqBWgpQX6BVKU+l2QSRU7EIiUW4//IPNRJwf9NXEOfaMJlOb5BNbKB1MliT8yhm1sLMO6MbnWanDxm/JcNe1Dj+egguH9im9m1OSnKS8UHlEhtN+JEqVKCGJ83c9LESkRPLZjxBlXoR4QXbjqzWH+GS2XpGLypzKlpdtY28rWB1JDOH4eEv3oh4O2f0GSVZhBXGLuANi1EF9zPaOYyN2WL7ecxB8WzpHHiXpgEh62WWpq3kQNV+GyF4AolRNafcz/YuF0JQkIvchS7F743rFP2mqq07wpmD8fRJZCxaBmPePf8mrUb9AVGqkDaV2G4ihi6TSIvwKIuIiJdpzx8wCMn/HdCw2Yl5DRqKMVBrJ2DUQr70PkUHgrsfK6MF8LmDzOjs+W+x02WJ6lNyCkzHwFJCl5yH6TJaNTkOIrLJLUrcdxPR9Wl8UlvQ1aDqu/HXILAqHVIjSJUtq4uSVs6jceT3M6XshOnwMUaQSRM7Ckkzryt+fyh7Qr9Lju6rJgXobZBN0PTWoXj3xiUmqbHu2PQYSM7xpl1T/iMDnbJGeXS7tQLT4Wqc5UL2MSWy75zTTfSgvzA/0lhzdk56HjPoN4q3BEOXqaZ2EVYRoKx2Zabt1GBLTySCiHbIkqseZjIg3cTKYQM9Ij/P5ApI4F53TxEMFwSsFzZStVrt+EPlLQeQKk61tS4heE6RXdUgTBxFDNCPxid3qxqbgbDGvoy3FSfedJgnzwXu9h0P/fk4W1uwzupscn5bevIfZqvI4L3m23LJSfimvk3dCvDMconx92cDQ0mJVILrK7tGiM9qwiCy5opA+P52Gv7ZvwZlz53Dm9BmcPXvWSSoRKo/Tvv0S9HDl+23+ngvbHQmVKb8gMvuIrAvjIarLOpFHdu+zy15bieoQTbrIHpA8/r0k38Xn/Q0s2QT33ri+xMVGTVtlO6UtkXWfIRC309h/Rogu0/xzOeNTDzhuc10KSi+thzvhD4iPJkE066a9ScpzrmLSw2ylu+XEFzzWykNmVIfMeCn/QxenPeI0uygE7XFKUlx41l+YtqGQEiZKpb3xqTYO8kRppaQ6r0K8Owri201asRSOlM0KJyXyoqykNBKzgOyCs8UkNLNAFx6V3WHZDS4vC+/eu3DrQ9klMa2N2jWJy304LHmyZHSmoRMZ07JZw1dDvDlYNx40ZpNT6qyC9Mq7ytZ9xkFtwEyqTLgUJ5En6aLzUDz6yCN4LFNmPProo3jsscecpBKh8njUt58hQwZJen9EJU7T/rhsyS6oXNlGpktnou90iFY9IEq/6LORInqMtGJjaa/vQbw9QnVTFTEvvejvIcXGRs10sGySvcAmLfUUpHzS45t23L8coh02tvfgOkY90BYyzZWb6TzkLKobiFJ19PE+U+X99uv6zQ4FOy6BnCYSOk7ESR7nxjRGnAwmzvP/nMKTOXLrtTRJCZxhUqRpKEyKZCxEksN/gnh9EESVl7RCiUgKS8+vagvtqveeIrvSmyF+lPGujtSExF382BYkF6a5T8qngqWCIiOZ+gvEEw/i1lselQXp8zptw7HjtIXDUHeLlsT6UHrVrT/Q0yfowUCOwroiVJcta5ehuqEgQuSuOo+5cutqxk1TkijMG59FmULlJBXL+B1R5wXbdmISAZOEvfwghZt3EuKrVRA9v9deGtlPmOzm55CeWqfBurElm7HtMSah+3L3vGMXld4M9QZqoifbpy8imHXYTntMQmG4UaBPeDTurOdKD1spPezj+hx31XnIjtMTmzrHxDl4sTfGqT7VkoyIN3GaA+IEIs5MWXNIxZz0LxUXSBSJ+rqe5JmRArilIcKdLRX9lSTTt4ZBNOykPVNqqZ7PI8mnjN4fMEtfF4hgAgmH4TTQfT2SitSDzfvk/qCRyoDue31+9CeJsbkPxUeD1l+v0+mksavm72pvksav5p3ye9/mwwBzDMjWFwvFTY1Gn/EYO2wIJk6ejIkTJ2LSxElOUolQeZBMnjQZWZ9/Tpb5xujd3VDCnpY5Ds5DPGSTPPZJv+ncimtRbdu2SVs4jEfm5yAe/h/c82wdHT/VXSJ7ip+2ZryxiZ/DsZhddaqzzA3kBKk8Im76IaHw0cY4I9LGU3W7q37t8iU8nbeAbGFO+JVjF5atAFYCt7j0m4iFl1fjh0ikfPo41NR9EAPnQUzeFXUc0i44W8z70e+VlyGmHNJx/CQ9uZXS+DbckKT9s/pk8CMtftDGab/tYcdrC4fh1tbsqlNBc5oDjd2a1wfaUngi2n6TcePkX1ZpOKQ2PPfM07LXsUGXO5WdaYe23QSyITM8iUmobDsUhhpe215iEo6P4iBinPErbhG34o5W42Tdu6zHXMdu1BPhpx6NSp6xSb95H97aD7NsMcMHEjMsxUFCDsoXi7B7w3rFQeo1X7sQkhAJJk5vAjx5nNkMj9Nr0YzMB9q3FcPK8Vpd3zEiT+7GxLWrzuG4a/JBX9xye0aIEfv1Sit0jDzOdh10F2vgdr/Bx/Ye5r14q/Lg29rptfNsH7fTTi00GUuvsTi6bbNa5INWSuKPvTlJeeHyOHnyJB5+6EE9xmnaqm0rtgSzAdsWbDGvDSWePUF3y8ev0DZ/530Qd0RfjOXWF/tru4tLXQiWpmDXBwsfLAwtnkKe9+Cl+H3NKsU/vLRcciHBxMnvqm/fugW3PviIfqrOD4eCianEQEqzw5NwS2OTkHldMDHjIEOetlZNPbrl9ocgmsqudOdBEBWrK0P535zNddfIjj+29zHvZ19rH7clWDjakrGQ99pnEo5u3ajKILkXNnCIGexI0DbvC8/pjwxSw0xDQWaZ2uUcyAZsO4jpuBmfbVO2cBh2bNachahaC+KZnBBlG8h60BSiwVsQLbtBFK+Mfzf6In5OhJkm+7pgaY+NkC4VaS7BC1Xq4LRspNT8WaV3u0SSDgkmTvY4T544jnseelQPBpN7H9vxx7hIfOMzFc9d3uEzIJ5/OmoLW7YdxOKr0R9uxfe+CRW+Lw0ZkE4XH8Pt7XrizKH9nu5pHUuH1AFaN4Abs4WzZ6nVe8TInzR5kj3xeJ5ZtikhZn2g3+R5ks3TQybqffFTfqon8e2qJ6bQPVl39PrxhE247f8ewep5s5Xeycs3hw2TA/EmToJJnkSfLevXhShUzf+kjJ4E20o3ySiQ2Aqzz9nH7GsCiR2eyJNa0Z9l93fsBojPF0JMOeKf9hRXjzY+YqfJFgpD6SAdqlfzLkFkyooZo0cq3aeEsTjEDLMXRli2aD4yZ34K4tV+uj7wU/K4PhBhe7FtKD5ixsfCvTja8hAZT9oPlE47zlBiX29LsDCqsfE9ICNOIRLv9AUeyfRfzP5hstJxlHqQjHUh3sTJFZaJk42lNZFnrtIQC49plzrQggasrJQUSg9tiSypleVpI+wRpJSQbnhslMaKaez1u98gnngBfbu/q3Qc4fvqpUPqBJVN+HX9nZAD+/ejSY2qENkKQXQfI+0r3P/tLSYGk5wC2YN9LLHFJiz7uB0+MYTjNe/L5M06IU+YdEXr+/Ycp6Zf1a5QDn/+8YfSLZOmT+tp4+EQgZleifHZgM96f4AHnn4Bok1viGUX/AtpBDKUQGIr2C5MuxBCiR0/x2EWFqfJDm/HFUzsa2Pat+/B6eDWlbxhMpjZhyFqt0eWZ17A1HFjlG6VsRhfu3QEmnoQpT7c8H+sjrBi3lzUrVoFt+SUBNqkq56bTF4o1Q3qUdBzAW4wbScjkNg2ZdqWLWa4QLYXV7Hjt+9hh7GvN8XMM+mAZtQwX4zfpif85wpDrcqVsHjmdE+fVA9IyVHqQTLWhQQRpwKl18f1pue554/taNukEe7NkV8aytv6LQdqZbm1JaUFmr9oKzqm33bhpYTYhmBLsDBMlGQ0FIanYNFQAU12rt4Kj2fPg+5vvIbT//ytdGq2sMnbvjrECUaDRvWB6wRhx6YN6NW5I/IULKxfQW4oSfTzZRCLfYt9MJGak8Jjmu8YzL7M4zHZqm3PZlh73742mNhhuSdHeTCHAKhHxe/lU1d82RWIYdL2m0qyLFQO2fMXVPa/7dd1nv5Il/wALiUdhwQTp6rAiuz95KlaAx/27fwDH0pDyVWgoFKGeKWvbm2pC09EQeM+1LqQss3WxzYSuxDTgpiGY7es5FXyRPjVsvv25UqIxu+o788XLVIMX/Xvg5N/HfX0aI7l6NbVO+WQyqCKxlcnWHjKEoO80V+XLUGfrp1RtnRp/G8OWqegmvSwekJ8Mg/ih8O6jhCh7PNteQI5HbdtiuuMXW9C1SE7jBnWPhZI+J6cHrsOU1opzfYEfnIQZvwFMXAhRMteEMVq4Z6cBVC6dCn0eutN/LJkEa5fu+rpi3SXWkiTkGDi9DzOKIYSnUAjwq/jJ9ld6frqK8hbSLa2uYtAVGkJ8c4YPd+NlE0KZcWaD2rYM+Wx0kDEmpISxWiMdNI5MhpesIAaCmphac3DUZsgOg6FqNAEGfKEoUTxYuj3ThdsXvOToVxtMNGMhXXukDpB5aQ2/srN+3a9YJw4dACLpk1G7y6dULNKZTxL3yzKVRSiTEPplcoGtfsE2RNZpz6forw2Hs4hoTpD9kW2RbZG3ioTLBGgaZ9MbOzFeltTzHO+fZuYKW6qn/RsgBwffgOQ00RCaaRr556CGLEe4v1J0jnoBlGusaz/RfF0/kKoLrvgPaRXOW/SePy1b6+tFo8wTfs3CTOlyDPhxGnAnwlNnvSbP58QJXvSeHb8th5jBn2C1g3rI6/0sG6X5EGtjmjaHeK98RDf/g4x/6w2Am6pqDCoO0MGQgVGBcdvTngtsFHQwcRsJQO1mIGEDYmvN42HDIfSxOkkoXSSZ0nv7tMbJF1HQ9TvBFG0Ku6WLWtY8eJ4o1VzTBv1NQ7t3GlqR00xIp0xYTqkbQSq6GbdoM8926DvSv25cQPmjB+LT9/rhpYN6qFM2bJ4qmAY/ie3rCvFq0OUrKu7+m0HSvsaI7v8y3W9mXoAYt5p/b45ESe/Qsk2ytONWNhmWbgnyFsKT/bMr2GS3dN8yvln9L3onp+vkE7QWIh2gzQ5lqynbJ3S+oQkyFJlyqJZvboY8G5XzBo3Gjs2/IIr58/Z2VY2r3RikaWNQMeSE4lKnASd0eitLY9/RiNROi+PH/xzB+ZN/B4fd++GZnVfRL7CRfB4fmkg+Urqibl13tQG0mcmxNA16ouPYtFFTWRErmQUVND74C98FtNIyBAoLLWQJLxPWzpnhjXjYKOi49xlIjJdcA5iwi6IL6ThfDhVdjt6y7R2kIZdU3W7nywQhuKlSqNN4wYY8tGHWD5rOo4d2GdpILjBBNKlQ9qAXW4xCXuibAPBcPncWRzduxvrly3G9NEj8fXH/fBO2zZoWe9F1K5dG/mLFMXzefIhsyTYB4qVh8gv60/uYrIelYYoXFn28lpAVGoOUfs1aaevQ9RqL4+1iSp0jGyYwlSWYau21Msd5i/ti6sEMhargCcKFsNz8l5UV2vVqoUWst6+1boVhvfvgx+kQ7Bu4Xwc2b0Ll86dtrPhgTnB7Flpx8sn9CuAvlIaiU6cNgJl0jQUNXZnB/Dh3MkT2LZuLRbLLsxQSTo9Xm+PhnVqo1SxoshTqgweylMI/86eX7ZsslBL19WtXIVmEI1k16bNAIjXvoDoNAKiyyiIHhN1V+EDSW5DVmjyJRmyWm8HLdXnSbpLj7fzN/LaryDafwbxcj+IutJbLN8UolRdvdxXWHn8P3nvTAWKIl+JkupTCS0bNcSHHV/HyE/6Y8XsGdj526+4dCaw0ZAOOP/Os3QwYZOpaSexQcS1q7h4+hROHT6IvVu3YMvqVfh1yUKsnDMTy2ZMxY/jxylimzV2FCYNH4bPP+yBz3v2wGc+mTxiGGZ+O0oS89cq7LIZ09S1v8g4KK49WzaruC/8cwrhV/3jkDGBelF2XoKRYKBjqQ1JSpx2KxGTmC2uUmwo5YVfx9kTx3BIeqp//LIWq3+cJbu9IzDui4H4qn9vDHy/O96TRPtKowayJayDpjVroGntGnixUgXVOtaqUwe1ZAutpM6LqFmjOupXrYSmdWqiSa3qaFX/RbwqvcReXTphkDQmatm/H/KZMqhfFs2XpPiL+pb8BXriTSuzxAD63jh/3THQAHdM4pC+YZd3bIR7b0xC9I48fzk0ZL1JAihSJM+R0uKrvyRxtXVTUjuSlDhjA1aSqTBTgTydIzycC0QXRmoCpZrGqcwWldOdVgzBIe3BtCubeEionjDJmmLaKTsrptjn7es5Xvt+N5OtpzhxEmwDMKF+U4H49gMbRtQW2DaKwEbA1/mvt+OwrzXjiMl4zHzY5+xwDg7xgW1nJsxjofZtm7Tt074m0P0YgeJMr0hx4rSVbe57v/XBKMeV+AaRKUSg44Hii4tEuTbIPShx/NvbN651cEgqRLPRAHYbaj8mBLrG/G2LeU16R4oTp4ODg0NagyNOBwcHhzjCEaeDg4NDHOGI08HBwSGOcMTp4ODgEEc44nRwcHCIIxxxOjg4OMQRjjgdHBwc4ghHnA4ODg5xhCNOBwcHhzjCEaeDg4NDHOGI08HBwSGOcMTp4ODgEEc44nRwcHCIIxxxOjg4OMQRjjgdHBwc4ghHnA4ODg5xhCNOBwcHhzjCEaeDg4NDHOGI08HBwSGOcMTp4ODgEEc44nRwcHCIIxxxOjg4OMQRjjgdHBwc4ghHnA4ODg5xhCNOBwcHhzjCEaeDg4NDHOGI08HBwSGOcMTp4ODgEEc44nRwcHCII/4/jvsZa9ATrZgAAAAASUVORK5CYII=>