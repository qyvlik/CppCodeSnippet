# JSON like value

Safe for use pointer.

```

GarbageCollector gc;
ValueCreator valueCreator(&gc);
Value* val = valueCreator.create(13.0);

trigger::ValueHandle valHandle = valueCreator.valueHandle(val);

std::cout << "Value Handle :[" << valHandle.get() << "]" << std::endl;

valHandle->print();

std::cout << std::endl;

gc.connectionManager()->printConnections();

gc.collect(true);                           // delete val

std::cout << "Value Handle :[" << valHandle.get() << "]" << std::endl;                          // 0x0

gc.connectionManager()->printConnections();

return 0;

```