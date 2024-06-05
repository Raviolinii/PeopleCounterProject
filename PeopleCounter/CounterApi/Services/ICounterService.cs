namespace CounterApi.Services;

public interface ICounterService
{
    public int GetPeopleCount();
    public void PersonEntered();
    public void PersonExited();
}
