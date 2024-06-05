using CounterApi.Classes;
using CounterApi.Services;
using MassTransit;
using RabbitMQ.Client;
using RabbitMQ.Client.Events;
using System.Text;
using System.Text.Json;

namespace CounterApi.RabbitConsumer;

public class CounterConsumer : IConsumer<CounterMessage>
{
    private readonly ICounterService _counterService;
    public CounterConsumer(ICounterService counterService)
    {
        _counterService = counterService;
    }

    public Task Consume(ConsumeContext<CounterMessage> context)
    {

        Console.WriteLine($"Received message: {context}");

        if(context.Message.Entered)
            _counterService.PersonEntered();
        else
            _counterService.PersonExited();

        return Task.CompletedTask;
    }
}
