
## 1. Start from what you already have

You plug the ray into the sphere:

[
(O_x + kD_x)^2 + (O_y + kD_y)^2 + (O_z + kD_z)^2 = R^2
]

Bring everything to one side:

[
(O_x + kD_x)^2 + (O_y + kD_y)^2 + (O_z + kD_z)^2 - R^2 = 0
]

---

## 2. Expand each square

Use:
[
(a + b)^2 = a^2 + 2ab + b^2
]

Apply to each term:

[
(O_x^2 + 2k O_x D_x + k^2 D_x^2)
]
[

* (O_y^2 + 2k O_y D_y + k^2 D_y^2)
  ]
  [
* (O_z^2 + 2k O_z D_z + k^2 D_z^2)

- R^2 = 0
  ]

---

## 3. Group by powers of k

Now collect terms:

### k² terms:

[
k^2 (D_x^2 + D_y^2 + D_z^2)
]

### k terms:

[
k (2(O_x D_x + O_y D_y + O_z D_z))
]

### constant terms:

[
O_x^2 + O_y^2 + O_z^2 - R^2
]

---

## 4. Now it matches a quadratic

ak^2 + bk + c = 0

So you can **identify directly**:

### a:

[
a = D_x^2 + D_y^2 + D_z^2
]

👉 that’s just:

```
a = dot(D, D)
```

---

### b:

[
b = 2 (O_x D_x + O_y D_y + O_z D_z)
]

👉 that’s:

```
b = 2 * dot(O, D)
```

---

### c:

[
c = O_x^2 + O_y^2 + O_z^2 - R^2
]

👉 that’s:

```
c = dot(O, O) - R^2
```

---

## 5. Important: when sphere is NOT at origin

Right now you're using `(0,0,0)` as center.

In your actual code, the sphere has a center `C`, so you must shift:

```
oc = ray.origin - center;
```

Then everything becomes:

```
a = dot(D, D);
b = 2 * dot(oc, D);
c = dot(oc, oc) - radius * radius;
```

---

## 6. Intuition (this is the key insight)

* **a** comes from how fast the ray moves → direction squared
* **b** comes from how the ray is oriented relative to the sphere
* **c** is how far the ray origin already is from the sphere

So the quadratic is basically:

> “Starting from O, moving along D, when do I land exactly R units away from the center?”

---