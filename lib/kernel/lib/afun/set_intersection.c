mixed *set_intersection(mixed *a, mixed *b, varargs int r) {
   mixed *c;

   c = a & b;

   return r ? set_remove_repeats(c) : c;
}
