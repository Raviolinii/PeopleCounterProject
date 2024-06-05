using MassTransit;

namespace CounterApi.RabbitConsumer;

public static class MassTransitExtension
{
    public static void AddMasstransit(this IServiceCollection collection, RabbitMQConfig config)
    {
        collection.AddMassTransit(x =>
        {
            x.AddConsumer<CounterConsumer>();
            x.UsingRabbitMq((context, cfg) =>
            {
                cfg.Host(new Uri(config.Host), host =>
                {
                    host.Username(config.UserName);
                    host.Password(config.Password);
                });

                cfg.ReceiveEndpoint(config.Queue, ep =>
                {
                    ep.PrefetchCount = 1;
                    ep.ConfigureConsumer<CounterConsumer>(context);
                });
                cfg.ClearSerialization();
                cfg.UseRawJsonSerializer(isDefault: true);
            });
        });

        //collection.AddMassTransitHostedService();
    }
}
