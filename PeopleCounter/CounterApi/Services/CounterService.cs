namespace CounterApi.Services;

public class CounterService : ICounterService
{
    private static int peopleCount;

    public CounterService()
    {
        peopleCount = 0;
    }

    public int GetPeopleCount() => peopleCount;
    public void PersonEntered() => peopleCount++;
    public void PersonExited()
    {
        if (peopleCount <= 0)
        {
            return;
        }
        peopleCount--;
    }
}
