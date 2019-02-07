typedef bool (*CmpFunction)(void*, void*);

void swap(void* a, void* b)   {
   void* temp = a;
   a = b;
   b = temp;
}

void quickSort(void** array, int size, CmpFunction compare)
{
   int pivot;
   int b = 1;
   int t = size - 1;
   if (size < 2) {
      return;
   }
   swap(&array[0], &array[size/2]);
   pivot = array[0];
   while(b <= t)    {
      while(t >= b && array[t] >= pivot )   {
         t--;
      }
      while(b <= t && array[b] < pivot) {
         b++; 
      }
      if ( b < t)   {
         swap(&array[b++], &array[t--]);
      }
   }
   swap(&array[0], &array[t]);
   quickSort(array, t);
   quickSort(array + t + 1, size – t - 1);
}
